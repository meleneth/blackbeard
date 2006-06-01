#include "mnzb.hpp"

mNZB::mNZB()
{
}

mNZB::~mNZB()
{
}

void mNZB::save_postset(PostSet *set)
{
    this->set = set;
}

void mNZB::load_postset(PostSet *set)
{
    this->set = set;
}

