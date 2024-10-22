#include "CppUnitTest.h"

#include <string>
#include <sstream>
#include <locale>
#include <stdexcept>

#include "../Deadzone/source/ResourceManager/ResourceManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFont
{
    TEST_CLASS(TestFont)
    {
    public:
        TEST_METHOD(Antonio)
        {
            Logger::WriteMessage("Antonio-Bold.ttf");

            try
            {
                ResourceManager::loadFont("../../../Deadzone/resources/fonts/Antonio-Bold.ttf", 24, "Antonio");

                Assert::AreEqual(static_cast<size_t>(128), ResourceManager::getFont("Antonio").Characters.size()); // 128
            }
            catch (const std::runtime_error& err)
            {
                Assert::Fail(widen(err.what()).c_str());
            }
            catch (...)
            {
                Assert::Fail(L"Other errors");
            }
        }

    private:
        std::wstring widen(const std::string& str)
        {
            std::wostringstream wstm;
            const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
            for (size_t i = 0; i < str.size(); ++i)
            {
                wstm << ctfacet.widen(str[i]);
            }

            return wstm.str();
        }
    };
}

