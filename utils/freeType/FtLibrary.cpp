#include "FtLibrary.h"
#include <utils/debug.h>
#include "utils/StringMaker.h"

namespace ft {

int Library::initialized = 0;

Library::Library()
{
    if (!initialized) {
        const auto error = FT_Init_FreeType(&library);
        if (error)
            // throw Exception("Couldn't init FreeType engine", error);
            Error("Couldn't init FreeType engine {}", std::to_string(error));
    }

    ++initialized;
}

Library::~Library()
{
    if (initialized) {
        if (--initialized == 0) {
            FT_Done_FreeType(library);
            library = nullptr;
        }
    }
}

std::string Library::getVersionString() const
{
    if (!library)
        return "";

    FT_Int major;
    FT_Int minor;
    FT_Int patch;
    FT_Library_Version(library, &major, &minor, &patch);
    return StringMaker() << major << "." << minor << "." << patch;
}

}
