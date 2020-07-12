#include "./XM.h"

const double SECONDS_PER_SAMPLE = 1;
const uint16_t RATE = 48000;
const uint16_t BUFFER_STEP_COUNT = 5;

class Error : exception
{
public:
	string message;
	Error(string message) : message(message)
	{
	}
};

xm_context_t *XM::create_context_from_file(uint32_t rate, const char *filename)
{
	ifstream inputFile;
	xm_context_t *result;
	streampos begin, end;
	char *data;

	inputFile.open(filename, ios::in & ios::binary);
	if (!inputFile.is_open())
	{
		throw Error("Could not open the file.");
	}

	begin = inputFile.tellg();
	inputFile.seekg(0, ios::end);
	end = inputFile.tellg();
	inputFile.seekg(0, ios::beg);

	data = new char[end - begin];

	inputFile.read(data, end - begin);

	int returnStatus = xm_create_context_safe(&result, data, end - begin, rate);

	if (returnStatus == 1)
	{
		throw Error("Module data is not sane.");
	}
	else if (returnStatus == 2)
	{
		throw Error("Memory allocation failed.");
	}

	inputFile.close();
	delete[] data;
	return result;
}

void play(float *largeBuffer, uint32_t numSamples, uint32_t rate)
{

	// bool played = al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
}

ALLEGRO_SAMPLE *XM::copyToBuffer(xm_context_t *xm)
{
	uint bufferSize = xm->rate;
	vector<shared_ptr<float[]>> buffers;

	for (uint32_t k = 0; xm_get_loop_count(xm) == 0; k++ )
	{
		shared_ptr<float[]> buffer(new float[bufferSize]);

		xm_generate_samples(xm, buffer.get(), bufferSize / 2);
		buffers.push_back(buffer);
	}

	auto size = bufferSize * buffers.size();

	float *destination = new float[size];

	uint i = 0;
	for (shared_ptr<float[]> buffer : buffers)
	{
		memcpy(destination + (i * bufferSize), buffer.get(), bufferSize * sizeof(float));
		i++;
	}

	auto sample = al_create_sample(destination, buffers.size() * bufferSize / 2, RATE, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2, true);

	return sample;
}