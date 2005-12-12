#include "pararchive.hpp"

ParArchive::ParArchive()
{
}

ParArchive::~ParArchive()
{
}

void ParArchive::add_parfile(string filename)
{
    par_filenames.push_back(filename);
}

Uint32 ParArchive::exists_in_par(string filename)
{
}

Uint32 ParArchive::is_corrupt(string filename)
{
}

