#include "WebRTCWrapper.h"

WebRTCWrapper::WebRTCWrapper(): _apm(NULL), _enable(false) {

    init(false, false, false, 48000, 1, 48000, 1);
} 

WebRTCWrapper::~WebRTCWrapper() {
	
    _enable = false;

    if (_apm) {
		delete _apm; _apm = NULL;
	}
}

bool WebRTCWrapper::init(bool use_noise_suppression, bool use_gain_control, bool use_echo_cancellation, int32_t near_end_samplerate, uint32_t near_end_channels, int32_t far_end_samplerate, uint32_t far_end_channels) {

    _enable = false;

    qWarning() << "WebRTCWrapper::init() use_noise_suppression=" << use_noise_suppression << " use_gain_control=" << use_gain_control << " use_echo_cancellation=" << use_echo_cancellation;

	_frame_near.num_channels_ = near_end_channels;
	_frame_near.sample_rate_hz_ = near_end_samplerate;
	_frame_near.samples_per_channel_ = near_end_samplerate/100; // Math for 10ms frames
	_frame_far.num_channels_ = far_end_channels;
	_frame_far.sample_rate_hz_ = far_end_samplerate;
	_frame_far.samples_per_channel_ = far_end_samplerate/100; // Math for 10ms frames

    if (_apm) {
        delete _apm; _apm = NULL;
    }

	if ( NULL == _apm ) {
		_apm = AudioProcessing::Create();
	}

	AudioProcessing::ChannelLayout near_end_layout, far_end_layout;

	switch(near_end_channels) {
		case 1:
			near_end_layout = AudioProcessing::kMono;
			break;
		case 2:
			near_end_layout = AudioProcessing::kStereo;
			break;
		default:
			qWarning() << "Unsupported near end layout - channels: " << near_end_channels;
			return false;
	}

	switch(far_end_channels) {
		case 1:
			far_end_layout = AudioProcessing::kMono;
			break;
		case 2:
			far_end_layout = AudioProcessing::kStereo;
			break;
		default:
			qWarning() << "Unsupported far end layout - channels: " << far_end_channels;
			return false;
	}


    if (use_noise_suppression || use_gain_control || use_echo_cancellation) {
        _enable = true;
    }

	_apm->Initialize(near_end_samplerate, far_end_samplerate, near_end_samplerate, near_end_layout, far_end_layout, near_end_layout);

	//const size_t frameLength = _frame_near.samples_per_channel_;
	//apm->echo_cancellation()->set_suppression_level( EchoCancellation::SuppressionLevel::kHighSuppression );
	_apm->echo_cancellation()->enable_drift_compensation( false );

	_apm->echo_cancellation()->Enable( use_echo_cancellation );
	_apm->echo_cancellation()->enable_delay_logging( use_echo_cancellation );
    _apm->echo_cancellation()->set_suppression_level( EchoCancellation::SuppressionLevel::kHighSuppression); //kLowSuppression, kModerateSuppression, kHighSuppression

    Config extraconfig;
    extraconfig.Set<webrtc::DelayAgnostic>(new webrtc::DelayAgnostic(true));
    extraconfig.Set<webrtc::ExtendedFilter>(new webrtc::ExtendedFilter(true));
    //extraconfig.Set<RefinedAdaptiveFilter>(new RefinedAdaptiveFilter(true));
    _apm->SetExtraOptions(extraconfig);

	_apm->noise_suppression()->set_level( NoiseSuppression::Level::kHigh );
	_apm->noise_suppression()->Enable(use_noise_suppression);
	_apm->high_pass_filter()->Enable(use_noise_suppression);


	_apm->gain_control()->set_analog_level_limits( 0, 255 );
	_apm->gain_control()->set_mode( GainControl::Mode::kAdaptiveDigital );
	_apm->gain_control()->Enable(use_gain_control);

	return true;
}

void WebRTCWrapper::update_audiobuffer_near_end(uint32_t channels, int32_t samplerate, short* data, uint32_t length, uint32_t latency) {
    if (! _enable) return;

    qWarning() << "WebRTCWrapper: update_audiobuffer_near_end() getting shorts! channels=" << channels << " samplerate=" << samplerate << " length=" << length << " latency=" << latency;

	_frame_near.num_channels_ = channels;
	_frame_near.sample_rate_hz_ = samplerate;
	_frame_near.samples_per_channel_ = samplerate/100;

	assert(length == _frame_near.samples_per_channel_ * channels);
	memcpy(_frame_near.data_, data, sizeof(short) * length * channels);

	_apm->set_stream_delay_ms( latency );

    int32_t rc = _apm->ProcessStream(&_frame_near);

	if (rc != 0) {
		qWarning() << "WebRTCWrapper: ProcessStream() Error=" << rc;
	} else {
        memcpy(data, _frame_near.data_, sizeof(short) * length * channels);
    }
}

void WebRTCWrapper::update_audiobuffer_near_end(uint32_t channels, int32_t samplerate, float** data, uint32_t length, uint32_t latency) {
    if (! _enable) return;
	
    qWarning() << "WebRTCWrapper: update_audiobuffer_near_end() getting floats! channels=" << channels << " samplerate=" << samplerate << " latency=" << latency;

	_frame_near.num_channels_ = channels;
	_frame_near.sample_rate_hz_ = samplerate;
	_frame_near.samples_per_channel_ = samplerate/100;

	assert(length == _frame_near.samples_per_channel_ * channels);
	memcpy(_frame_near.data_, data, sizeof(float) * length * channels);

	AudioProcessing::ChannelLayout layout;
	switch(channels) {
		case 1:
			layout = AudioProcessing::kMono;
			break;
		case 2:
			layout = AudioProcessing::kStereo;
			break;
		default:
			qWarning() << "WebRTCWrapper: Unsupported layout - channels: " << channels;
			return;
	}

    _apm->set_stream_delay_ms( latency );

	int32_t rc = _apm->ProcessStream(data, _frame_near.samples_per_channel_, samplerate, layout, samplerate, layout, data);

	if (rc != 0) {
		qWarning() << "WebRTCWrapper: ProcessStream() Error=" << rc;
	} else {
        memcpy(data, _frame_near.data_, sizeof(float) * length * channels);
    }
}

void WebRTCWrapper::update_audiobuffer_far_end(uint32_t channels, int32_t samplerate, short* data, uint32_t length, uint32_t latency) {
    if (! _enable) return;

    qWarning() << "WebRTCWrapper: update_audiobuffer_far_end() getting shorts channels=" << channels << " samplerate=" << samplerate << " latency=" << latency;
    
    _frame_far.num_channels_ = channels;
	_frame_far.sample_rate_hz_ = samplerate;
	_frame_far.samples_per_channel_ = samplerate/100;

	assert(length == _frame_far.samples_per_channel_ * channels);
	memcpy(_frame_far.data_, data, sizeof(short) * length * channels);

    _apm->set_stream_delay_ms( latency );

	int32_t rc = _apm->ProcessReverseStream(&_frame_far);
	if (rc != 0) {
		qWarning() << "WebRTCWrapper: ProcessReverseStream() Error=" << rc;
	} else {
        memcpy(data, _frame_far.data_, sizeof(short) * length * channels);
    }

}

bool WebRTCWrapper::eventFilter(QObject* obj, QEvent* event) {
    if (event->type()==QEvent::KeyPress) {
        qWarning() << "WebRTCWrapper: Getting a keyboard event." << obj; 
    }

    return false;
}

