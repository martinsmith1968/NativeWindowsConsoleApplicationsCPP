//
// pch.h
//

#pragma once

// From : https://stackoverflow.com/questions/68193449/disable-warnings-from-google-test
#pragma warning( push )
#pragma warning( disable : 26495 )
#pragma warning( disable : 26812 )

#include "gtest/gtest.h"

#pragma warning( pop )
