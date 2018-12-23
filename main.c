#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

	// Read values from stdin and process them.
	while (1) {

		// ^C will cause scanf to exit with -1.
		double x;
		int status = scanf("%lf", &x);
		if (status == -1) break;
		if (status != 1 || x < -1.0 || x > 1.0) {
			fprintf(stderr, "bad input from stdin\n");
			FLAC__stream_encoder_delete(encoder);
			return 1;
		}

		FLAC__int32 y = x * 8388608;

		// Process it.
		ok = FLAC__stream_encoder_process_interleaved(encoder, &y, 1);
		if (!ok) {
			fprintf(stderr, "failed to process buffer\n");
			FLAC__stream_encoder_delete(encoder);
			return 1;
		}

	}

	// Finish.
	ok = FLAC__stream_encoder_finish(encoder);
	if (!ok) {
		fprintf(stderr, "failed to finish encoding\n");
		FLAC__stream_encoder_delete(encoder);
		return 1;
	}

	FLAC__stream_encoder_delete(encoder);
	printf("all done!\n");
	return 0;

}

