#ifndef _WEBRTCWRAPPER_HEADER_INCLUDED_
#define _WEBRTCWRAPPER_HEADER_INCLUDED_

//#define WEBRTC_POSIX // will be added by project
//#define WEBRTC_AUDIO_PROCESSING_ONLY_BUILD // will be added by project

#include <stdio.h>
#include <assert.h>
#include "webrtc/common.h"
#include "webrtc/modules/interface/module_common_types.h"
#include "webrtc/modules/audio_processing/include/audio_processing.h"

#include <QtGlobal>

using namespace webrtc;

class WebRTCWrapper {
    public:
        WebRTCWrapper();
        ~WebRTCWrapper();

        bool init(bool use_noise_suppression, bool use_gain_control, bool use_echo_cancellation, int32_t near_end_samplerate, uint32_t near_end_channels, int32_t far_end_samplerate, uint32_t far_end_channels);

        void update_audiobuffer_near_end(uint32_t channels, int32_t samplerate, short* data, uint32_t length, uint32_t latency);

        void update_audiobuffer_near_end(uint32_t channels, int32_t samplerate, float** data, uint32_t length, uint32_t latency);

        void update_audiobuffer_far_end(uint32_t channels, int32_t samplerate, short* data, uint32_t length, uint32_t latency);

    protected:
        bool eventFilter(QObject* obj, QEvent* event);
    private:
	    AudioProcessing* _apm;
	    AudioFrame _frame_near, _frame_far;
        bool _enable;
};


#endif
