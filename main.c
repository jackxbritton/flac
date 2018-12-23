#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <FLAC/stream_encoder.h>

int main(int argc, char *argv[]) {

	FLAC__StreamEncoder *encoder = FLAC__stream_encoder_new();
	if (encoder == NULL) {
		fprintf(stderr, "failed to create encoder\n");
		return 1;
	}

	FLAC__bool ok = true;
	ok &= FLAC__stream_encoder_set_verify(encoder, true);
	ok &= FLAC__stream_encoder_set_compression_level(encoder, 5);
	ok &= FLAC__stream_encoder_set_channels(encoder, 1);
	ok &= FLAC__stream_encoder_set_bits_per_sample(encoder, 24);
	ok &= FLAC__stream_encoder_set_sample_rate(encoder, 44100);
	ok &= FLAC__stream_encoder_set_total_samples_estimate(encoder, 44100);

	if (!ok) {
		fprintf(stderr, "failed to configure encoder\n");
		FLAC__stream_encoder_delete(encoder);
		return 1;
	}

	// Point the encoder at a file to write to.
	ok = FLAC__stream_encoder_init_file(encoder, "out.flac", NULL, NULL)
	   == FLAC__STREAM_ENCODER_INIT_STATUS_OK;
	if (!ok) {
		fprintf(stderr, "failed to initialize encoder file\n");
		FLAC__stream_encoder_delete(encoder);
		return 1;
	}

	// TODO Define a crude sine wave.
	int buf_len = 44100;
	FLAC__int32 *buf = malloc(buf_len * sizeof(FLAC__int32));
	if (buf == NULL) return 1;
	for (int i = 0; i < buf_len; i++) {
		double frequency = 110.0;
		buf[i] = sin(2.0*M_PI * (double) i/(frequency/buf_len)) * 8388608;
	}

	// Process it.
	ok = FLAC__stream_encoder_process_interleaved(encoder, buf, buf_len);
	if (!ok) {
		fprintf(stderr, "failed to process buffer\n");
		FLAC__stream_encoder_delete(encoder);
		return 1;
	}

	// Finish.
	ok = FLAC__stream_encoder_finish(encoder);
	if (!ok) {
		fprintf(stderr, "failed to finish encoding\n");
		FLAC__stream_encoder_delete(encoder);
		return 1;
	}

	FLAC__stream_encoder_delete(encoder);
	return 0;

}

