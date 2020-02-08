// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.h
//
/**
    \file   plugincore.h
    \author Will Pirkle
    \date   17-September-2018
    \brief  base class interface file for ASPiK plugincore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#ifndef __pluginCore_h__
#define __pluginCore_h__

#include "pluginbase.h"


// **--0x7F1F--**

// --- Plugin Variables controlID Enumeration 

enum controlID {
	m_fFreqOsc0 = 12,
	m_uOscType = 2,
	m_fDutyCOsc0 = 13,
	m_uLFOSel = 7,
	m_fLFOFreq = 17,
	m_fLFODutyC = 18,
	m_uFLOon = 8,
	m_uOscType2 = 4,
	m_uOsc0Mute = 3,
	m_fFreqOsc1 = 14,
	m_fDutyCOsc1 = 15,
	m_uOsc1Mute = 5
};

	// **--0x0F1F--**

/**
\class PluginCore
\ingroup ASPiK-Core
\brief
The PluginCore object is the default PluginBase derived object for ASPiK projects.
Note that you are fre to change the name of this object (as long as you change it in the compiler settings, etc...)


PluginCore Operations:
- overrides the main processing functions from the base class
- performs reset operation on sub-modules
- processes audio
- processes messages for custom views
- performs pre and post processing functions on parameters and audio (if needed)

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class PluginCore : public PluginBase
{
public:
    PluginCore();

	/** Destructor: empty in default version */
    virtual ~PluginCore(){}

	// --- PluginBase Overrides ---
	//
	/** this is the creation function for all plugin parameters */
	bool initPluginParameters();

	/** called when plugin is loaded, a new audio file is playing or sample rate changes */
	virtual bool reset(ResetInfo& resetInfo);

	/** one-time post creation init function; pluginInfo contains path to this plugin */
	virtual bool initialize(PluginInfo& _pluginInfo);

	// --- preProcess: sync GUI parameters here; override if you don't want to use automatic variable-binding
	virtual bool preProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** process frames of data */
	virtual bool processAudioFrame(ProcessFrameInfo& processFrameInfo);

	// --- uncomment and override this for buffer processing; see base class implementation for
	//     help on breaking up buffers and getting info from processBufferInfo
	//virtual bool processAudioBuffers(ProcessBufferInfo& processBufferInfo);

	/** preProcess: do any post-buffer processing required; default operation is to send metering data to GUI  */
	virtual bool postProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo);

	/** this can be called: 1) after bound variable has been updated or 2) after smoothing occurs  */
	virtual bool postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** this is ony called when the user makes a GUI control change */
	virtual bool guiParameterChanged(int32_t controlID, double actualValue);

	/** processMessage: messaging system; currently used for custom/special GUI operations */
	virtual bool processMessage(MessageInfo& messageInfo);

	/** processMIDIEvent: MIDI event processing */
	virtual bool processMIDIEvent(midiEvent& event);

	/** specialized joystick servicing (currently not used) */
	virtual bool setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData);

	/** create the presets */
	bool initPluginPresets();

	// --- BEGIN USER VARIABLES AND FUNCTIONS -------------------------------------- //
	//	   Add your variables and methods here
	
	double m_preProcessR = 0.0;
	double m_preProcessL = 0.0;

	double m_outputR = 0.0;
	double m_outputL = 0.0;

	double m_OscOut0 = 0.0;
	double m_OscOut1 = 0.0;


	double m_fs = audioProcDescriptor.sampleRate;

	//Oscilator 1 Variables
	double m_moduloOsc0 = 0.0;
	double m_foOsc0 = 440.0;
	double m_incOsc0 = m_foOsc0 / m_fs;

	//Oscilator 2 Variables
	double m_moduloOsc1 = 0.0;
	double m_foOsc1 = 440.0;
	double m_incOsc1 = m_foOsc1 / m_fs;

	//LFO variables
	double m_moduloLFO = 0.0;
	double m_foLFO = 440.0;
	double m_incLFO = m_foLFO / m_fs;



	//Refresh modulo
	double reModulo(double modulo, double inc)
	{
		//check for modulo wrap test 
		if (modulo >= 1.0)
		{
			modulo -= 1.0; //wrap
		}

		modulo += inc;

		//check for modulo wrap test 
		if (modulo >= 1.0)
		{
			modulo -= 1.0; //wrap
		}

		return modulo;
	}

	//Trival_saw_oscilator
	double t_saw(double modulo)
	{

		//unipolar to bipolar
		double wave = 2.0 * modulo - 1.0;

		return wave;
	}

	//Trival square wave
	double t_sqw(double modulo, double DutyC)
	{

		//unipolar to bipolar
		double wave = modulo > DutyC / 100.0 ? -1.0 : 1.0;

		return wave;
	}

	//Trival triagle wave
	double t_tri(double modulo)
	{
		//unipolar to bipolar
		double wave = 2.0 * fabs(2.0 * modulo - 1.0) - 1.0;

		return wave;
	}

	//polyBLEP
	double doPolyBlep(double modulo, double inc, double height, bool risingEdge)
	{
		double PolyBLEP = 0.0;

		//--- LEFT side of discontinuity
		if (modulo >= 1.0 - inc)
		{
			//--- calculate distance
			double t = (modulo - 1.0) / inc;

			//--- calculate residual
			PolyBLEP = height * (t * t + 2.0 * t + 1.0);
		}

		//--- RIGHT side of discontinuty
		else if (modulo <= inc)
		{
			//--- calculate distance
			double t = modulo / inc;

			//--- calculate residual
			PolyBLEP = height * (2.0 * t - t * t - 1.0);
		}

		//--- subtract fo falling, add for rising edge
		if (!risingEdge)
		{
			PolyBLEP *= -1.0;
		}

		return PolyBLEP;
	}

	//Signal Select
	double waveSel(double moduloSel,double incSel, double DutyCSel, double OscMode)
	{
		double outputL = 0.0;
		double outputR = 0.0;

		if (compareEnumToInt(m_uOscTypeEnum::Saw, OscMode))
		{ 
			outputL = t_saw(moduloSel) + doPolyBlep(moduloSel, incSel, 1.0, false);
			outputR = t_saw(moduloSel) + doPolyBlep(moduloSel, incSel, 1.0, false);
		}
		else if (compareEnumToInt(m_uOscTypeEnum::Saw_raw, OscMode))
		{
			outputL = t_saw(moduloSel);
			outputR = t_saw(moduloSel);
		}
		else if (compareEnumToInt(m_uOscTypeEnum::Square_Raw, OscMode))
		{
			outputL = t_sqw(moduloSel,DutyCSel);
			outputR = t_sqw(moduloSel,DutyCSel);
		}
		else if (compareEnumToInt(m_uOscTypeEnum::Square, OscMode))
		{
			double preSquare = t_saw(moduloSel) + doPolyBlep(moduloSel, incSel, 1.0, false);
			double preSquareNeg = t_saw((moduloSel - DutyCSel / 100)) + doPolyBlep(moduloSel - DutyCSel / 100, incSel, 1.0, false);
			outputL = preSquare - preSquareNeg;
			outputR = preSquare - preSquareNeg;
		}
		else if (compareEnumToInt(m_uOscTypeEnum::Triangle, OscMode))
		{
			outputL = t_tri(moduloSel);
			outputR = t_tri(moduloSel);
		}
		else if (compareEnumToInt(m_uOscTypeEnum::Sin, OscMode))
		{
			outputL = sin(moduloSel * 2 * 3.1417);
			outputR = sin(moduloSel * 2 * 3.1416);
		}

		return outputL, outputR;

	}


	// --- END USER VARIABLES AND FUNCTIONS -------------------------------------- //

private:
	//  **--0x07FD--**

	// --- Continuous Plugin Variables 
	double m_fFreqOsc0 = 0.0;
	double m_fDutyCOsc0 = 0.0;
	double m_fLFOFreq = 0.0;
	double m_fLFODutyC = 0.0;
	double m_fFreqOsc1 = 0.0;
	double m_fDutyCOsc1 = 0.0;

	// --- Discrete Plugin Variables 
	int m_uOscType = 0;
	enum class m_uOscTypeEnum { Saw_raw,Saw,Square_Raw,Square,Triangle,Sin };	// to compare: if(compareEnumToInt(m_uOscTypeEnum::Saw_raw, m_uOscType)) etc... 

	int m_uLFOSel = 0;
	enum class m_uLFOSelEnum { Saw_raw,Saw,Square_Raw,Square,Triangle,Sin };	// to compare: if(compareEnumToInt(m_uLFOSelEnum::Saw_raw, m_uLFOSel)) etc... 

	int m_uFLOon = 0;
	enum class m_uFLOonEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(m_uFLOonEnum::SWITCH_OFF, m_uFLOon)) etc... 

	int m_uOscType2 = 0;
	enum class m_uOscType2Enum { Saw_raw,Saw,Square_Raw,Square,Triangle,Sin };	// to compare: if(compareEnumToInt(m_uOscType2Enum::Saw_raw, m_uOscType2)) etc... 

	int m_uOsc0Mute = 0;
	enum class m_uOsc0MuteEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(m_uOsc0MuteEnum::SWITCH_OFF, m_uOsc0Mute)) etc... 

	int m_uOsc1Mute = 0;
	enum class m_uOsc1MuteEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(m_uOsc1MuteEnum::SWITCH_OFF, m_uOsc1Mute)) etc... 

	// **--0x1A7F--**
    // --- end member variables

public:
    /** static description: bundle folder name

	\return bundle folder name as a const char*
	*/
    static const char* getPluginBundleName();

    /** static description: name

	\return name as a const char*
	*/
    static const char* getPluginName();

	/** static description: short name

	\return short name as a const char*
	*/
	static const char* getShortPluginName();

	/** static description: vendor name

	\return vendor name as a const char*
	*/
	static const char* getVendorName();

	/** static description: URL

	\return URL as a const char*
	*/
	static const char* getVendorURL();

	/** static description: email

	\return email address as a const char*
	*/
	static const char* getVendorEmail();

	/** static description: Cocoa View Factory Name

	\return Cocoa View Factory Name as a const char*
	*/
	static const char* getAUCocoaViewFactoryName();

	/** static description: plugin type

	\return type (FX or Synth)
	*/
	static pluginType getPluginType();

	/** static description: VST3 GUID

	\return VST3 GUID as a const char*
	*/
	static const char* getVSTFUID();

	/** static description: 4-char code

	\return 4-char code as int
	*/
	static int32_t getFourCharCode();

	/** initalizer */
	bool initPluginDescriptors();

};




#endif /* defined(__pluginCore_h__) */
