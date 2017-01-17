/*
 * Copyright (C) 2010, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include "modules/webaudio/GainNode.h"
#include "modules/webaudio/AudioNodeInput.h"
#include "modules/webaudio/AudioNodeOutput.h"
#include "modules/webaudio/GainOptions.h"
#include "platform/audio/AudioBus.h"

namespace blink {

GainHandler::GainHandler(AudioNode& node,
                         float sampleRate,
                         AudioParamHandler& gain)
    : AudioHandler(NodeTypeGain, node, sampleRate),
      m_lastGain(1.0),
      m_gain(gain),
      m_sampleAccurateGainValues(ProcessingSizeInFrames)  // FIXME: can probably
                                                          // share temp buffer
                                                          // in context
{
  addInput();
  addOutput(1);

  initialize();
}

PassRefPtr<GainHandler> GainHandler::create(AudioNode& node,
                                            float sampleRate,
                                            AudioParamHandler& gain) {
  return adoptRef(new GainHandler(node, sampleRate, gain));
}

void GainHandler::process(size_t framesToProcess) {
  // FIXME: for some cases there is a nice optimization to avoid processing
  // here, and let the gain change happen in the summing junction input of the
  // AudioNode we're connected to.  Then we can avoid all of the following:

  AudioBus* outputBus = output(0).bus();
  DCHECK(outputBus);

  if (!isInitialized() || !input(0).isConnected()) {
    outputBus->zero();
  } else {
    AudioBus* inputBus = input(0).bus();

    if (m_gain->hasSampleAccurateValues()) {
      // Apply sample-accurate gain scaling for precise envelopes, grain
      // windows, etc.
      DCHECK_LE(framesToProcess, m_sampleAccurateGainValues.size());
      if (framesToProcess <= m_sampleAccurateGainValues.size()) {
        float* gainValues = m_sampleAccurateGainValues.data();
        m_gain->calculateSampleAccurateValues(gainValues, framesToProcess);
        outputBus->copyWithSampleAccurateGainValuesFrom(*inputBus, gainValues,
                                                        framesToProcess);
        // Update m_lastGain so if the timeline ever ends, we get
        // consistent data for the smoothing below.  (Without this,
        // m_lastGain was the last value before the timeline started
        // procesing.
        m_lastGain = gainValues[framesToProcess - 1];
      }
    } else {
      // Apply the gain with de-zippering into the output bus.
      if (!m_lastGain && m_lastGain == m_gain->value()) {
        // If the gain is 0 (and we've converged on dezippering), just zero the
        // bus and set the silence hint.
        outputBus->zero();
      } else {
        outputBus->copyWithGainFrom(*inputBus, &m_lastGain, m_gain->value());
      }
    }
  }
}

// FIXME: this can go away when we do mixing with gain directly in summing
// junction of AudioNodeInput
//
// As soon as we know the channel count of our input, we can lazily initialize.
// Sometimes this may be called more than once with different channel counts, in
// which case we must safely uninitialize and then re-initialize with the new
// channel count.
void GainHandler::checkNumberOfChannelsForInput(AudioNodeInput* input) {
  DCHECK(context()->isAudioThread());
  ASSERT(context()->isGraphOwner());

  DCHECK(input);
  DCHECK_EQ(input, &this->input(0));
  if (input != &this->input(0))
    return;

  unsigned numberOfChannels = input->numberOfChannels();

  if (isInitialized() && numberOfChannels != output(0).numberOfChannels()) {
    // We're already initialized but the channel count has changed.
    uninitialize();
  }

  if (!isInitialized()) {
    // This will propagate the channel count to any nodes connected further
    // downstream in the graph.
    output(0).setNumberOfChannels(numberOfChannels);
    initialize();
  }

  AudioHandler::checkNumberOfChannelsForInput(input);
}

// ----------------------------------------------------------------

GainNode::GainNode(BaseAudioContext& context)
    : AudioNode(context),
      m_gain(AudioParam::create(context, ParamTypeGainGain, 1.0)) {
  setHandler(
      GainHandler::create(*this, context.sampleRate(), m_gain->handler()));
}

GainNode* GainNode::create(BaseAudioContext& context,
                           ExceptionState& exceptionState) {
  DCHECK(isMainThread());

  if (context.isContextClosed()) {
    context.throwExceptionForClosedState(exceptionState);
    return nullptr;
  }

  return new GainNode(context);
}

GainNode* GainNode::create(BaseAudioContext* context,
                           const GainOptions& options,
                           ExceptionState& exceptionState) {
  GainNode* node = create(*context, exceptionState);

  if (!node)
    return nullptr;

  node->handleChannelOptions(options, exceptionState);

  if (options.hasGain())
    node->gain()->setValue(options.gain());

  return node;
}

AudioParam* GainNode::gain() const {
  return m_gain;
}

DEFINE_TRACE(GainNode) {
  visitor->trace(m_gain);
  AudioNode::trace(visitor);
}

}  // namespace blink
