#pragma once

#include "AbstractVideoProcessor.h"

AbstractVideoProcessor::AbstractVideoProcessor() {

}

AbstractVideoProcessor::~AbstractVideoProcessor() {

}

void AbstractVideoProcessor::setOutput(DeckLinkOutputDevice* output) {
	this->output = output;
}