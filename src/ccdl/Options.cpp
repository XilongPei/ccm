//=========================================================================
// Copyright (C) 2018 The C++ Component Model(CCM) Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#include "Options.h"

#include <stdio.h>

namespace ccdl {

Options::Options(
    /* [in] */ int argc,
    /* [in] */ char** argv)
    : mOptionNumber(argc)
    , mShowUsage(false)
    , mFormatError(false)
    , mShouldCompile(false)
    , mShouldSaveMetadata(false)
    , mShouldGenerate(false)
    , mGenForComponent(false)
    , mMetadataFileType(0)
{
    Parse(argc, argv);
}

void Options::Parse(
    /* [in] */ int argc,
    /* [in] */ char** argv)
{
    mProgram = argv[0];

    int i = 1;
    while (i < argc) {
        const char* string = argv[i++];
        if (!strcmp("--help", string)) {
            mShowUsage = true;
        }
        else if (!strcmp("-c", string)) {
            mShouldCompile = true;
        }
        else if (!strcmp("-d", string)) {
            mOutputDir = argv[i++];
        }
        else if (!strcmp("-g", string)) {
            mShouldGenerate = true;
            mGenForComponent = true;
            mMetadataFileType = SO_FILE;
        }
        else if (!strcmp("-m", string)) {
            mMetadataFileType = METADATA_FILE;
        }
        else if (!strcmp("-o", string)) {
            mShouldSaveMetadata = true;
            mMetadataOuputFile = argv[i++];
        }
        else if (!strcmp("-u", string)) {
            mGenForComponent = false;
        }
        else {
            mInputFile = string;
        }
    }
}

void Options::ShowUsage()
{
    printf("Compile a .cdl file to metadata, or generate C++ codes from metadata."
           "Usage: ccdl [options] file\n"
           "Options:\n"
           "  --help            Display command line options\n"
           "  -c                Compile the .cdl file to metadata\n"
           "  -d <directory>    Place generated C++ codes into <directory>\n"
           "  -g                Generate C++ codes for component\n"
           "  -m                Make the input file as a metadata file, .so is default\n"
           "  -o <file>         Place the metadata into <file>\n"
           "  -u                Together with -g to generate C++ codes for component user");
}

}
