#include "CppUnitTest.h"

#include <string>
#include <sstream>
#include <locale>
#include <stdexcept>
#include <fstream>

#include <nlohmann/json.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestConfigJSON
{
    TEST_CLASS(TestConfigJSON)
    {
    public:
        TEST_METHOD(Input)
        {
            Logger::WriteMessage("input.json");

            std::ifstream file("../../../Deadzone/config/input.json");
            if (!file.is_open())
            {
                Assert::Fail(L"Failed to open: input.json");
            }

            nlohmann::json json;
            try
            {
                file >> json;
            }
            catch (const nlohmann::json::parse_error& err)
            {
                Assert::Fail(widen(err.what()).c_str());
            }
            catch (...)
            {
                Assert::Fail(L"Other errors");
            }

            file.close();
        }

        TEST_METHOD(Game)
        {
            Logger::WriteMessage("game.json");

            std::ifstream file("../../../Deadzone/config/game.json");
            if (!file.is_open())
            {
                Assert::Fail(L"Failed to open: game.json");
            }

            nlohmann::json json;
            try
            {
                file >> json;
            }
            catch (const nlohmann::json::parse_error& err)
            {
                Assert::Fail(widen(err.what()).c_str());
            }
            catch (...)
            {
                Assert::Fail(L"Other errors");
            }

            file.close();
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

