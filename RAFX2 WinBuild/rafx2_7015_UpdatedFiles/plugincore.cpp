// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.cpp
//
/**
    \file   plugincore.cpp
    \author Will Pirkle
    \date   17-September-2018
    \brief  Implementation file for PluginCore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#include "plugincore.h"
#include "plugindescription.h"


/**
\brief PluginCore constructor is launching pad for object initialization

Operations:
- initialize the plugin description (strings, codes, numbers, see initPluginDescriptors())
- setup the plugin's audio I/O channel support
- create the PluginParameter objects that represent the plugin parameters (see FX book if needed)
- create the presets
*/
PluginCore::PluginCore()
{
    // --- describe the plugin; call the helper to init the static parts you setup in plugindescription.h
    initPluginDescriptors();

    // --- default I/O combinations
	// --- for FX plugins
	if (getPluginType() == kFXPlugin)
	{
		addSupportedIOCombination({ kCFMono, kCFMono });
		addSupportedIOCombination({ kCFMono, kCFStereo });
		addSupportedIOCombination({ kCFStereo, kCFStereo });
	}
	else // --- synth plugins have no input, only output
	{
		addSupportedIOCombination({ kCFNone, kCFMono });
		addSupportedIOCombination({ kCFNone, kCFStereo });
	}

	// --- for sidechaining, we support mono and stereo inputs; auxOutputs reserved for future use
	addSupportedAuxIOCombination({ kCFMono, kCFNone });
	addSupportedAuxIOCombination({ kCFStereo, kCFNone });

	// --- create the parameters
    initPluginParameters();

    // --- create the presets
    initPluginPresets();
}

/**
\brief create all of your plugin parameters here

\return true if parameters were created, false if they already existed
*/
bool PluginCore::initPluginParameters()
{
	if (pluginParameterMap.size() > 0)
		return false;

    // --- Add your plugin parameter instantiation code bewtween these hex codes
	// **--0xDEA7--**


	// --- Declaration of Plugin Parameter Objects 
	PluginParameter* piParam = nullptr;

	// --- continuous control: Freq Osc 1
	piParam = new PluginParameter(controlID::m_fFreqOsc0, "Freq Osc 1", "Hz", controlVariableType::kDouble, 80.000000, 2000.000000, 440.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fFreqOsc0, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Oscillator 1
	piParam = new PluginParameter(controlID::m_uOscType, "Oscillator 1", "Saw raw,Saw,Square Raw,Square,Triangle,Sin", "Sin");
	piParam->setBoundVariable(&m_uOscType, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Duty Cycle
	piParam = new PluginParameter(controlID::m_fDutyCOsc0, "Duty Cycle", "%", controlVariableType::kDouble, 20.000000, 80.000000, 50.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fDutyCOsc0, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: LFO wave
	piParam = new PluginParameter(controlID::m_uLFOSel, "LFO wave", "Saw raw,Saw,Square Raw,Square,Triangle,Sin", "Sin");
	piParam->setBoundVariable(&m_uLFOSel, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: LFO Freq
	piParam = new PluginParameter(controlID::m_fLFOFreq, "LFO Freq", "Hz", controlVariableType::kDouble, 0.200000, 5.000000, 0.200000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fLFOFreq, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO Duty Cycle
	piParam = new PluginParameter(controlID::m_fLFODutyC, "LFO Duty Cycle", "Units", controlVariableType::kDouble, 20.000000, 80.000000, 50.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fLFODutyC, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: FLO
	piParam = new PluginParameter(controlID::m_uFLOon, "FLO", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&m_uFLOon, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Oscillator 2
	piParam = new PluginParameter(controlID::m_uOscType2, "Oscillator 2", "Saw raw,Saw,Square Raw,Square,Triangle,Sin", "Sin");
	piParam->setBoundVariable(&m_uOscType2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Mute Osc 1
	piParam = new PluginParameter(controlID::m_uOsc0Mute, "Mute Osc 1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&m_uOsc0Mute, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Freq Osc 2
	piParam = new PluginParameter(controlID::m_fFreqOsc1, "Freq Osc 2", "Hz", controlVariableType::kDouble, 80.000000, 1500.000000, 440.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fFreqOsc1, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Duty Cicle Osc 2
	piParam = new PluginParameter(controlID::m_fDutyCOsc1, "Duty Cicle Osc 2", "Units", controlVariableType::kDouble, 20.000000, 80.000000, 50.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fDutyCOsc1, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Mute Osc 2
	piParam = new PluginParameter(controlID::m_uOsc1Mute, "Mute Osc 2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&m_uOsc1Mute, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Volume
	piParam = new PluginParameter(controlID::m_fVolumeOut, "Volume", "db", controlVariableType::kDouble, -15.000000, 5.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fVolumeOut, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Pan
	piParam = new PluginParameter(controlID::m_fPanOut, "Pan", "Units", controlVariableType::kDouble, 0.000000, 1.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fPanOut, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: LPF On
	piParam = new PluginParameter(controlID::m_uLPFon, "LPF On", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&m_uLPFon, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: HPF On
	piParam = new PluginParameter(controlID::m_uHPFon, "HPF On", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&m_uHPFon, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: LPF Freq
	piParam = new PluginParameter(controlID::m_fLPFfo, "LPF Freq", "Hz", controlVariableType::kDouble, 3500.000000, 5000.000000, 4000.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fLPFfo, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: HPF freq
	piParam = new PluginParameter(controlID::m_fHPFfo, "HPF freq", "Hz", controlVariableType::kDouble, 80.000000, 500.000000, 80.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fHPFfo, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: BPF1 On
	piParam = new PluginParameter(controlID::m_uBPFon_1, "BPF1 On", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&m_uBPFon_1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: BPF Freq 1
	piParam = new PluginParameter(controlID::m_fBPFfo_1, "BPF Freq 1", "Hz", controlVariableType::kDouble, 500.000000, 1500.000000, 750.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fBPFfo_1, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: BPF Band 1
	piParam = new PluginParameter(controlID::m_fBPFfb_1, "BPF Band 1", "Hz", controlVariableType::kDouble, 500.000000, 1500.000000, 750.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fBPFfb_1, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: BPF2 On
	piParam = new PluginParameter(controlID::m_uBPFon_2, "BPF2 On", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&m_uBPFon_2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: BPF Band 2
	piParam = new PluginParameter(controlID::m_fBPFfb_2, "BPF Band 2", "Hz", controlVariableType::kDouble, 500.000000, 1500.000000, 750.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fBPFfb_2, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: BPF Freq 2
	piParam = new PluginParameter(controlID::m_fBPFfo_2, "BPF Freq 2", "Hz", controlVariableType::kDouble, 1500.000000, 2500.000000, 1750.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fBPFfo_2, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: BPF3 On
	piParam = new PluginParameter(controlID::m_uBPFon_3, "BPF3 On", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&m_uBPFon_3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: BPF Freq 3
	piParam = new PluginParameter(controlID::m_fBPFfo_3, "BPF Freq 3", "Hz", controlVariableType::kDouble, 2500.000000, 3500.000000, 2750.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fBPFfo_3, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: BPF Band 3
	piParam = new PluginParameter(controlID::m_fBPFfb_3, "BPF Band 3", "Hz", controlVariableType::kDouble, 500.000000, 1500.000000, 750.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&m_fBPFfb_3, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- meter control: LEFT
	piParam = new PluginParameter(controlID::m_fLeftVolume, "LEFT", 10.00, 500.00, ENVELOPE_DETECT_MODE_RMS, meterCal::kLogMeter);
	piParam->setBoundVariable(&m_fLeftVolume, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: RIGHT
	piParam = new PluginParameter(controlID::m_fRightVolume, "RIGHT", 10.00, 500.00, ENVELOPE_DETECT_MODE_RMS, meterCal::kLogMeter);
	piParam->setBoundVariable(&m_fRightVolume, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- Aux Attributes
	AuxParameterAttribute auxAttribute;

	// --- RAFX GUI attributes
	// --- controlID::m_fFreqOsc0
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483651);
	setParamAuxAttribute(controlID::m_fFreqOsc0, auxAttribute);

	auxAttribute.reset(auxGUIIdentifier::midiControlData);
	auxAttribute.setUintAttribute(2147680256);
	setParamAuxAttribute(controlID::m_fFreqOsc0, auxAttribute);

	// --- controlID::m_uOscType
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::m_uOscType, auxAttribute);

	// --- controlID::m_fDutyCOsc0
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::m_fDutyCOsc0, auxAttribute);

	// --- controlID::m_uLFOSel
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::m_uLFOSel, auxAttribute);

	// --- controlID::m_fLFOFreq
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483655);
	setParamAuxAttribute(controlID::m_fLFOFreq, auxAttribute);

	// --- controlID::m_fLFODutyC
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::m_fLFODutyC, auxAttribute);

	// --- controlID::m_uFLOon
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741829);
	setParamAuxAttribute(controlID::m_uFLOon, auxAttribute);

	// --- controlID::m_uOscType2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::m_uOscType2, auxAttribute);

	// --- controlID::m_uOsc0Mute
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741829);
	setParamAuxAttribute(controlID::m_uOsc0Mute, auxAttribute);

	// --- controlID::m_fFreqOsc1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483663);
	setParamAuxAttribute(controlID::m_fFreqOsc1, auxAttribute);

	// --- controlID::m_fDutyCOsc1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::m_fDutyCOsc1, auxAttribute);

	// --- controlID::m_uOsc1Mute
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741829);
	setParamAuxAttribute(controlID::m_uOsc1Mute, auxAttribute);

	// --- controlID::m_fVolumeOut
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483654);
	setParamAuxAttribute(controlID::m_fVolumeOut, auxAttribute);

	// --- controlID::m_fPanOut
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483652);
	setParamAuxAttribute(controlID::m_fPanOut, auxAttribute);

	// --- controlID::m_uLPFon
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741829);
	setParamAuxAttribute(controlID::m_uLPFon, auxAttribute);

	// --- controlID::m_uHPFon
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741829);
	setParamAuxAttribute(controlID::m_uHPFon, auxAttribute);

	// --- controlID::m_fLPFfo
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::m_fLPFfo, auxAttribute);

	// --- controlID::m_fHPFfo
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::m_fHPFfo, auxAttribute);

	// --- controlID::m_uBPFon_1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741829);
	setParamAuxAttribute(controlID::m_uBPFon_1, auxAttribute);

	// --- controlID::m_fBPFfo_1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::m_fBPFfo_1, auxAttribute);

	// --- controlID::m_fBPFfb_1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483664);
	setParamAuxAttribute(controlID::m_fBPFfb_1, auxAttribute);

	// --- controlID::m_uBPFon_2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741829);
	setParamAuxAttribute(controlID::m_uBPFon_2, auxAttribute);

	// --- controlID::m_fBPFfb_2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483664);
	setParamAuxAttribute(controlID::m_fBPFfb_2, auxAttribute);

	// --- controlID::m_fBPFfo_2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::m_fBPFfo_2, auxAttribute);

	// --- controlID::m_uBPFon_3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741829);
	setParamAuxAttribute(controlID::m_uBPFon_3, auxAttribute);

	// --- controlID::m_fBPFfo_3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::m_fBPFfo_3, auxAttribute);

	// --- controlID::m_fBPFfb_3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483664);
	setParamAuxAttribute(controlID::m_fBPFfb_3, auxAttribute);

	// --- controlID::m_fLeftVolume
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::m_fLeftVolume, auxAttribute);

	// --- controlID::m_fRightVolume
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::m_fRightVolume, auxAttribute);


	// **--0xEDA5--**
   
    // --- BONUS Parameter
    // --- SCALE_GUI_SIZE
    PluginParameter* piParamBonus = new PluginParameter(SCALE_GUI_SIZE, "Scale GUI", "tiny,small,medium,normal,large,giant", "normal");
    addPluginParameter(piParamBonus);

	// --- create the super fast access array
	initPluginParameterArray();

    return true;
}

/**
\brief initialize object for a new run of audio; called just before audio streams

Operation:
- store sample rate and bit depth on audioProcDescriptor - this information is globally available to all core functions
- reset your member objects here

\param resetInfo structure of information about current audio format

\return true if operation succeeds, false otherwise
*/
bool PluginCore::reset(ResetInfo& resetInfo)
{
    // --- save for audio processing
    audioProcDescriptor.sampleRate = resetInfo.sampleRate;
    audioProcDescriptor.bitDepth = resetInfo.bitDepth;

    // --- other reset inits
    return PluginBase::reset(resetInfo);
}

/**
\brief one-time initialize function called after object creation and before the first reset( ) call

Operation:
- saves structure for the plugin to use; you can also load WAV files or state information here
*/
bool PluginCore::initialize(PluginInfo& pluginInfo)
{
	// --- add one-time init stuff here

	return true;
}

/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- syncInBoundVariables when preProcessAudioBuffers is called, it is *guaranteed* that all GUI control change information
  has been applied to plugin parameters; this binds parameter changes to your underlying variables
- NOTE: postUpdatePluginParameter( ) will be called for all bound variables that are acutally updated; if you need to process
  them individually, do so in that function
- use this function to bulk-transfer the bound variable data into your plugin's member object variables

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::preProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
    // --- sync internal variables to GUI parameters; you can also do this manually if you don't
    //     want to use the auto-variable-binding
    syncInBoundVariables();

    return true;
}

/**
\brief frame-processing method

Operation:
- decode the plugin type - for synth plugins, fill in the rendering code; for FX plugins, delete the if(synth) portion and add your processing code
- note that MIDI events are fired for each sample interval so that MIDI is tightly sunk with audio
- doSampleAccurateParameterUpdates will perform per-sample interval smoothing

\param processFrameInfo structure of information about *frame* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processAudioFrame(ProcessFrameInfo& processFrameInfo)
{
    // --- fire any MIDI events for this sample interval
    processFrameInfo.midiEventQueue->fireMidiEvents(processFrameInfo.currentFrame);

	// --- do per-frame updates; VST automation and parameter smoothing
	doSampleAccurateParameterUpdates();

    // --- decode the channelIOConfiguration and process accordingly
    //
	// --- Synth Plugin:
	// --- Synth Plugin --- remove for FX plugins
	if (getPluginType() == kSynthPlugin)
	{
		// --- midi
		uint32_t note = 0.0;
 
		// --- Oscilators

		//Refresh variables
		m_fs = audioProcDescriptor.sampleRate;
		//Osc1
		m_foOsc0 = m_fFreqOsc0;
		m_incOsc0 = m_foOsc0 / m_fs;
		//Osc2
		m_foOsc1 = m_fFreqOsc0;
		m_incOsc1 = m_foOsc1 / m_fs;
		//LFO
		m_foLFO = m_fLFOFreq;
		m_incLFO = m_foLFO / m_fs;

		//Filters

		
		//Refresh Volume
		m_volumeOut = pow(10, m_fVolumeOut / 20);

		//Select oscilators and LFO
		settings();	

		m_preProcessL = m_LFOmodulation * (0.5 * m_OscOut0 + 0.5 * m_OscOut1);
		m_preProcessR = m_LFOmodulation * (0.5 * m_OscOut0 + 0.5 * m_OscOut1);


		//Select filters and process the signal
		filterSelect();

		//Refresh de output
		m_outputL = m_postFilter_R;
		m_outputR = m_postFilter_R;
		
		
		//Increase module
		m_moduloOsc0 = reModulo(m_moduloOsc0, m_incOsc0);
		m_moduloOsc1 = reModulo(m_moduloOsc1, m_incOsc1);
		m_moduloLFO = reModulo(m_moduloLFO, m_incLFO);
		
		m_fRightVolume = m_volumeOut * (1 - m_fPanOut) * m_outputL;
		
		m_fLeftVolume = m_volumeOut * (m_fPanOut)*m_outputR;
				
		// --- output silence: change this with your signal render code
		processFrameInfo.audioOutputFrame[0] = m_volumeOut* (1 - m_fPanOut) *m_outputL;
		if (processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
			processFrameInfo.audioOutputFrame[1] = m_volumeOut*(m_fPanOut)*m_outputR;

		return true;	/// processed
	}


	

    // --- FX Plugin:
    if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFMono)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];

        return true; /// processed
    }

    // --- Mono-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];
        processFrameInfo.audioOutputFrame[1] = processFrameInfo.audioInputFrame[0];

        return true; /// processed
    }

    // --- Stereo-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFStereo &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];
        processFrameInfo.audioOutputFrame[1] = processFrameInfo.audioInputFrame[1];

        return true; /// processed
    }

    return false; /// NOT processed
}


/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- updateOutBoundVariables sends metering data to the GUI meters

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
	// --- update outbound variables; currently this is meter data only, but could be extended
	//     in the future
	updateOutBoundVariables();

    return true;
}

/**
\brief update the PluginParameter's value based on GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- use base class helper
    setPIParamValue(controlID, controlValue);

    // --- do any post-processing
    postUpdatePluginParameter(controlID, controlValue, paramInfo);

    return true; /// handled
}

/**
\brief update the PluginParameter's value based on *normlaized* GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param normalizedValue the new control value in normalized form
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo)
{
	// --- use base class helper, returns actual value
	double controlValue = setPIParamValueNormalized(controlID, normalizedValue, paramInfo.applyTaper);

	// --- do any post-processing
	postUpdatePluginParameter(controlID, controlValue, paramInfo);

	return true; /// handled
}

/**
\brief perform any operations after the plugin parameter has been updated; this is one paradigm for
	   transferring control information into vital plugin variables or member objects. If you use this
	   method you can decode the control ID and then do any cooking that is needed. NOTE: do not
	   overwrite bound variables here - this is ONLY for any extra cooking that is required to convert
	   the GUI data to meaninful coefficients or other specific modifiers.

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- now do any post update cooking; be careful with VST Sample Accurate automation
    //     If enabled, then make sure the cooking functions are short and efficient otherwise disable it
    //     for the Parameter involved
    /*switch(controlID)
    {
        case 0:
        {
            return true;    /// handled
        }

        default:
            return false;   /// not handled
    }*/

    return false;
}

/**
\brief has nothing to do with actual variable or updated variable (binding)

CAUTION:
- DO NOT update underlying variables here - this is only for sending GUI updates or letting you
  know that a parameter was changed; it should not change the state of your plugin.

WARNING:
- THIS IS NOT THE PREFERRED WAY TO LINK OR COMBINE CONTROLS TOGETHER. THE PROPER METHOD IS
  TO USE A CUSTOM SUB-CONTROLLER THAT IS PART OF THE GUI OBJECT AND CODE.
  SEE http://www.willpirkle.com for more information

\param controlID the control ID value of the parameter being updated
\param actualValue the new control value

\return true if operation succeeds, false otherwise
*/
bool PluginCore::guiParameterChanged(int32_t controlID, double actualValue)
{
	/*
	switch (controlID)
	{
		case controlID::<your control here>
		{

			return true; // handled
		}

		default:
			break;
	}*/

	return false; /// not handled
}

/**
\brief For Custom View and Custom Sub-Controller Operations

NOTES:
- this is for advanced users only to implement custom view and custom sub-controllers
- see the SDK for examples of use

\param messageInfo a structure containing information about the incoming message

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMessage(MessageInfo& messageInfo)
{
	// --- decode message
	switch (messageInfo.message)
	{
		// --- add customization appearance here
	case PLUGINGUI_DIDOPEN:
	{
		return false;
	}

	// --- NULL pointers so that we don't accidentally use them
	case PLUGINGUI_WILLCLOSE:
	{
		return false;
	}

	// --- update view; this will only be called if the GUI is actually open
	case PLUGINGUI_TIMERPING:
	{
		return false;
	}

	// --- register the custom view, grab the ICustomView interface
	case PLUGINGUI_REGISTER_CUSTOMVIEW:
	{

		return false;
	}

	case PLUGINGUI_REGISTER_SUBCONTROLLER:
	case PLUGINGUI_QUERY_HASUSERCUSTOM:
	case PLUGINGUI_USER_CUSTOMOPEN:
	case PLUGINGUI_USER_CUSTOMCLOSE:
	case PLUGINGUI_EXTERNAL_SET_NORMVALUE:
	case PLUGINGUI_EXTERNAL_SET_ACTUALVALUE:
	{

		return false;
	}

	default:
		break;
	}

	return false; /// not handled
}

/**
\brief process a MIDI event

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param event a structure containing the MIDI event data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMIDIEvent(midiEvent& event)
{
	return true;
}

/**
\brief (for future use)

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param vectorJoysickData a structure containing joystick data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData)
{
	return true;
}

/**
\brief use this method to add new presets to the list

NOTES:
- see the SDK for examples of use
- for non RackAFX users that have large paramter counts, there is a secret GUI control you
  can enable to write C++ code into text files, one per preset. See the SDK or http://www.willpirkle.com for details

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginPresets()
{
	// **--0xFF7A--**

	// --- Plugin Presets 
	int index = 0;
	PresetInfo* preset = nullptr;

	// --- Preset: Factory Preset
	preset = new PresetInfo(index++, "Factory Preset");
	initPresetParameters(preset->presetParameters);
	setPresetParameter(preset->presetParameters, controlID::m_fFreqOsc0, 440.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uOscType, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fDutyCOsc0, 50.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uLFOSel, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fLFOFreq, 0.200000);
	setPresetParameter(preset->presetParameters, controlID::m_fLFODutyC, 50.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uFLOon, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uOscType2, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uOsc0Mute, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fFreqOsc1, 439.999969);
	setPresetParameter(preset->presetParameters, controlID::m_fDutyCOsc1, 50.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uOsc1Mute, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fVolumeOut, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fPanOut, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::m_uLPFon, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uHPFon, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fLPFfo, 4000.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fHPFfo, 80.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uBPFon_1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fBPFfo_1, 750.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fBPFfb_1, 750.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uBPFon_2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fBPFfb_2, 750.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fBPFfo_2, 1750.000000);
	setPresetParameter(preset->presetParameters, controlID::m_uBPFon_3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fBPFfo_3, 2750.000000);
	setPresetParameter(preset->presetParameters, controlID::m_fBPFfb_3, 750.000000);
	addPreset(preset);


	// **--0xA7FF--**

    return true;
}

/**
\brief setup the plugin description strings, flags and codes; this is ordinarily done through the ASPiKreator or CMake

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginDescriptors()
{
    pluginDescriptor.pluginName = PluginCore::getPluginName();
    pluginDescriptor.shortPluginName = PluginCore::getShortPluginName();
    pluginDescriptor.vendorName = PluginCore::getVendorName();
    pluginDescriptor.pluginTypeCode = PluginCore::getPluginType();

	// --- describe the plugin attributes; set according to your needs
	pluginDescriptor.hasSidechain = kWantSidechain;
	pluginDescriptor.latencyInSamples = kLatencyInSamples;
	pluginDescriptor.tailTimeInMSec = kTailTimeMsec;
	pluginDescriptor.infiniteTailVST3 = kVSTInfiniteTail;

    // --- AAX
    apiSpecificInfo.aaxManufacturerID = kManufacturerID;
    apiSpecificInfo.aaxProductID = kAAXProductID;
    apiSpecificInfo.aaxBundleID = kAAXBundleID;  /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.aaxEffectID = "aaxDeveloper.";
    apiSpecificInfo.aaxEffectID.append(PluginCore::getPluginName());
    apiSpecificInfo.aaxPluginCategoryCode = kAAXCategory;

    // --- AU
    apiSpecificInfo.auBundleID = kAUBundleID;
	apiSpecificInfo.auBundleName = kAUBundleName;
	apiSpecificInfo.auBundleName = kAUBundleName;
	apiSpecificInfo.auBundleName = kAUBundleName;   /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.auBundleName = kAUBundleName;

    // --- VST3
    apiSpecificInfo.vst3FUID = PluginCore::getVSTFUID(); // OLE string format
    apiSpecificInfo.vst3BundleID = kVST3BundleID;/* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.enableVST3SampleAccurateAutomation = kVSTSAA;
	apiSpecificInfo.vst3SampleAccurateGranularity = kVST3SAAGranularity;

    // --- AU and AAX
    apiSpecificInfo.fourCharCode = PluginCore::getFourCharCode();

    return true;
}

// --- static functions required for VST3/AU only --------------------------------------------- //
const char* PluginCore::getPluginBundleName() { return kAUBundleName; }
const char* PluginCore::getPluginName(){ return kPluginName; }
const char* PluginCore::getShortPluginName(){ return kShortPluginName; }
const char* PluginCore::getVendorName(){ return kVendorName; }
const char* PluginCore::getVendorURL(){ return kVendorURL; }
const char* PluginCore::getVendorEmail(){ return kVendorEmail; }
const char* PluginCore::getAUCocoaViewFactoryName(){ return AU_COCOA_VIEWFACTORY_STRING; }
pluginType PluginCore::getPluginType(){ return kPluginType; }
const char* PluginCore::getVSTFUID(){ return kVSTFUID; }
int32_t PluginCore::getFourCharCode(){ return kFourCharCode; }
