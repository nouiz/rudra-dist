/**
 * GPFSSampleClient.h
 */
#ifndef RUDRA_IO_GPFSSAMPLECLIENT_H
#define RUDRA_IO_GPFSSAMPLECLIENT_H

#include "rudra/io/SampleReader.h"
#include "rudra/io/SampleClient.h"
#include <iostream>
#include <pthread.h>

#define GPFS_BUFFER_COUNT 1

namespace rudra {
class GPFSSampleClient: public SampleClient {
public:
	GPFSSampleClient(std::string name, bool threaded,
			SampleReader *sampleReader);
	//@Override
	void getLabelledSamples(float* samples, float* labels);
	size_t getSizePerLabel();
	~GPFSSampleClient();
protected:
	void producerThdFunc(void *args);
private:
	std::string clientName;
	bool threaded;
	SampleReader *sampleReader;
	float* X; // training data minibatch
	float* Y; // training label minibatch
	volatile bool finishedFlag;
	volatile int count;
	pthread_cond_t empty;
	pthread_cond_t fill;
	pthread_mutex_t mutex;
	pthread_t producerTID; //producer thread id
	void startProducerThd();
	static void* producerThdHook(void *args);
	void init();

};
}
#endif /* RUDRA_IO_GPFSSAMPLECLIENT_H */
