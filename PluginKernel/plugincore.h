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
	m_fFrecuencia = 0,
	m_uOscType = 10,
	m_fDutyC = 1
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

	double m_fs = audioProcDescriptor.sampleRate;
	double m_modulo = 0.0;
	double m_fo = 440.0;
	double m_PW = 50.0;
	double m_inc = m_fo / m_fs;
	double m_height = 0.0;


	//Trival_saw_oscilator
	double t_saw(void) 
	{
		//check for modulo wrap test 
		if (m_modulo >= 1.0)
		{
			m_modulo -= 1.0; //wrap
		}

		//incremet the couter
		m_modulo += m_inc/2;

		//modulo wrap test
		if (m_modulo >= 1.0)
		{
			m_modulo -= 1.0; //wrap
		}

		//unipolar to bipolar
		double wave = 2.0 * m_modulo - 1.0;

		return wave;
	}

	//Trival square wave
	double t_sqw(void)
	{
		//check for modulo wrap test 
		if (m_modulo >= 1.0)
		{
			m_modulo -= 1.0; //wrap
		}

		//incremet the couter
		m_modulo += m_inc/2;

		//modulo wrap test
		if (m_modulo >= 1.0)
		{
			m_modulo -= 1.0; //wrap
		}

		//unipolar to bipolar
		double wave = m_modulo > m_PW / 100.0 ? -1.0 : 1.0;

		return wave;
	}

	//Trival triagle wave
	double t_tri(void)
	{
		//check for modulo wrap test 
		if (m_modulo >= 1.0)
		{
			m_modulo -= 1.0; //wrap
		}

		//incremet the couter
		m_modulo += m_inc/2;

		//modulo wrap test
		if (m_modulo >= 1.0)
		{
			m_modulo -= 1.0; //wrap
		}

		//unipolar to bipolar
		double wave = 2.0*fabs(2.0*m_modulo-1.0)-1.0;

		return wave;
	}

	//polyBLEP
	double doPolyBlep(double modulo, double inc, double height, bool risingEdge)
	{
		double PolyBLEP = 0.0;

		//--- LEFT side of discontinuity
		if (modulo > 1.0-inc)
		{
			//--- calculate distance
			double t = (modulo - 1.0) / inc;

			//--- calculate residual
			PolyBLEP = height * (t * t + 2.0 * t + 1.0);
		}

		//--- RIGHT side of discontinuty
		else if (modulo < inc)
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


	// --- END USER VARIABLES AND FUNCTIONS -------------------------------------- //

private:
	//  **--0x07FD--**

	// --- Continuous Plugin Variables 
	double m_fFrecuencia = 0.0;
	double m_fDutyC = 0.0;

	// --- Discrete Plugin Variables 
	int m_uOscType = 0;
	enum class m_uOscTypeEnum { Saw_raw,Saw,Square,Triangle,Sin };	// to compare: if(compareEnumToInt(m_uOscTypeEnum::Saw_raw, m_uOscType)) etc... 

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
