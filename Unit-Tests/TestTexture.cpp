#include "CppUnitTest.h"

#include <string>
#include <sstream>
#include <locale>
#include <stdexcept>

#include "../DeadZone/source/ResourceManager/ResourceManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestTexture
{
    TEST_CLASS(TestTexture)
    {
    public:
        TEST_METHOD(OpenGLTest_png)
        {
            Logger::WriteMessage("OpenGL-test.png");
            TestTextureFunc("../../../DeadZone/resources/textures/OpenGL-test.png", "OpenGL-Logo", 2000u, 867u);
        }

        TEST_METHOD(floor0)
        {
            Logger::WriteMessage(".0");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.0.png", ".0", 128u, 128u);
        }

        TEST_METHOD(floor1)
        {
            Logger::WriteMessage(".1");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.1.png", ".1", 128u, 128u);
        }

        TEST_METHOD(floor2)
        {
            Logger::WriteMessage(".2");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.2.png", ".2", 128u, 128u);
        }

        TEST_METHOD(floor3)
        {
            Logger::WriteMessage(".3");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.3.png", ".3", 128u, 128u);
        }

        TEST_METHOD(floor4)
        {
            Logger::WriteMessage(".4");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.4.png", ".4", 128u, 128u);
        }

        TEST_METHOD(floor5)
        {
            Logger::WriteMessage(".5");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.5.png", ".5", 128u, 128u);
        }

        TEST_METHOD(floor6)
        {
            Logger::WriteMessage(".6");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.6.png", ".6", 128u, 128u);
        }

        TEST_METHOD(floor7)
        {
            Logger::WriteMessage(".7");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.7.png", ".7", 128u, 128u);
        }

        TEST_METHOD(floor8)
        {
            Logger::WriteMessage(".8");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.8.png", ".8", 128u, 128u);
        }

        TEST_METHOD(floor9)
        {
            Logger::WriteMessage(".9");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.9.png", ".9", 128u, 128u);
        }

        TEST_METHOD(floora)
        {
            Logger::WriteMessage(".a");
            TestTextureFunc("../../../DeadZone/resources/textures/floors/.a.png", ".a", 128u, 128u);
        }

        TEST_METHOD(M0)
        {
            Logger::WriteMessage("MO");
            TestTextureFunc("../../../DeadZone/resources/textures/walls/M0.png", "M0", 128u, 128u);
        }

        TEST_METHOD(M1)
        {
            Logger::WriteMessage("M1");
            TestTextureFunc("../../../DeadZone/resources/textures/walls/M1.png", "M1", 128u, 128u);
        }

        TEST_METHOD(M2)
        {
            Logger::WriteMessage("M2");
            TestTextureFunc("../../../DeadZone/resources/textures/walls/M2.png", "M2", 128u, 128u);
        }

        TEST_METHOD(weaponFrame)
        {
            Logger::WriteMessage("weaponFrame");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/weaponFrame.png", "weaponFrame", 128u, 128u);
        }

        TEST_METHOD(healthFrame)
        {
            Logger::WriteMessage("healthFrame");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/healthFrame.png", "healthFrame", 128u, 128u);
        }

        TEST_METHOD(staminaFrame)
        {
            Logger::WriteMessage("staminaFrame");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/staminaFrame.png", "staminaFrame", 128u, 128u);
        }

        TEST_METHOD(armorFrame)
        {
            Logger::WriteMessage("armorFrame");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/armorFrame.png", "armorFrame", 128u, 128u);
        }

        TEST_METHOD(redBar)
        {
            Logger::WriteMessage("redBar");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/red.png", "redBar", 128u, 128u);
        }

        TEST_METHOD(greenBar)
        {
            Logger::WriteMessage("greenBar");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/green.png", "greenBar", 128u, 128u);
        }

        TEST_METHOD(blueBar)
        {
            Logger::WriteMessage("blueBar");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/blue.png", "blueBar", 128u, 128u);
        }

        TEST_METHOD(fist0)
        {
            Logger::WriteMessage("fist0");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/fist0.png", "fist0", 128u, 128u);
        }

        TEST_METHOD(knife0)
        {
            Logger::WriteMessage("knife0");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/knife0.png", "knife0", 128u, 128u);
        }

        TEST_METHOD(pistol0)
        {
            Logger::WriteMessage("pistol0");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/pistol0.png", "pistol0", 128u, 128u);
        }

        TEST_METHOD(grenade0)
        {
            Logger::WriteMessage("grenade0");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/grenade0.png", "grenade0", 128u, 128u);
        }

        TEST_METHOD(shotgun0)
        {
            Logger::WriteMessage("shotgun0");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/shotgun0.png", "shotgun0", 128u, 128u);
        }

        TEST_METHOD(automated0)
        {
            Logger::WriteMessage("automated0");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/automated0.png", "automated0", 128u, 128u);
        }

        TEST_METHOD(automated1)
        {
            Logger::WriteMessage("automated1");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/automated1.png", "automated1", 128u, 128u);
        }

        TEST_METHOD(medicalKit0)
        {
            Logger::WriteMessage("medicalKit0");
            TestTextureFunc("../../../DeadZone/resources/textures/hud/medicalKit0.png", "medicalKit0", 128u, 128u);
        }

        TEST_METHOD(bullet0)
        {
            Logger::WriteMessage("bullet0");
            TestTextureFunc("../../../DeadZone/resources/textures/bullets/bullet0.png", "bullet0", 128u, 128u);
        }

        TEST_METHOD(bullet1)
        {
            Logger::WriteMessage("bullet1");
            TestTextureFunc("../../../DeadZone/resources/textures/bullets/bullet1.png", "bullet1", 128u, 128u);
        }

        TEST_METHOD(bullet2)
        {
            Logger::WriteMessage("bullet2");
            TestTextureFunc("../../../DeadZone/resources/textures/bullets/bullet2.png", "bullet2", 128u, 128u);
        }

        TEST_METHOD(bullet3)
        {
            Logger::WriteMessage("bullet3");
            TestTextureFunc("../../../DeadZone/resources/textures/bullets/bullet3.png", "bullet3", 128u, 128u);
        }

        TEST_METHOD(shop0)
        {
            Logger::WriteMessage("shop0");
            TestTextureFunc("../../../DeadZone/resources/textures/shops/shop0.png", "shop0", 128u, 128u);
        }

        TEST_METHOD(Tab1)
        {
            Logger::WriteMessage("Tab1");
            TestTextureFunc("../../../DeadZone/resources/textures/menus/shop/Tab1.png", "Tab1", 128u, 128u);
        }

        TEST_METHOD(Tab2)
        {
            Logger::WriteMessage("Tab2");
            TestTextureFunc("../../../DeadZone/resources/textures/menus/shop/Tab2.png", "Tab2", 128u, 128u);
        }

        TEST_METHOD(Tab3)
        {
            Logger::WriteMessage("Tab3");
            TestTextureFunc("../../../DeadZone/resources/textures/menus/shop/Tab3.png", "Tab3", 128u, 128u);
        }

        TEST_METHOD(Back)
        {
            Logger::WriteMessage("Back");
            TestTextureFunc("../../../DeadZone/resources/textures/menus/BackButton/Back.png", "Back", 128u, 128u);
        }

        TEST_METHOD(backHovered)
        {
            Logger::WriteMessage("backHovered");
            TestTextureFunc("../../../DeadZone/resources/textures/menus/BackButton/backHovered.png", "backHovered", 128u, 128u);
        }

        TEST_METHOD(button0Normal)
        {
            Logger::WriteMessage("button0Normal");
            TestTextureFunc("../../../DeadZone/resources/textures/menus/button0Normal.png", "button0Normal", 128u, 128u);
        }

        TEST_METHOD(button0Hovered)
        {
            Logger::WriteMessage("button0Hovered");
            TestTextureFunc("../../../DeadZone/resources/textures/menus/button0Hovered.png", "button0Hovered", 128u, 128u);
        }

        TEST_METHOD(menuScreen0)
        {
            Logger::WriteMessage("menuScreen0");
            TestTextureFunc("../../../DeadZone/resources/textures/menus/menuScreen0.png", "menuScreen0", 128u, 128u);
        }

        TEST_METHOD(noBackground)
        {
            Logger::WriteMessage("noBackground");
            TestTextureFunc("../../../DeadZone/resources/textures/menus/noBackground.png", "noBackground", 128u, 128u);
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

        void TestTextureFunc(const std::string& resourcePath, const std::string& textureName, unsigned int width, unsigned int height)
        {
            try
            {
                ResourceManager::loadTexture(resourcePath.c_str(), true, textureName);

                Assert::AreEqual(width, ResourceManager::getTexture(textureName).width);
                Assert::AreEqual(height, ResourceManager::getTexture(textureName).height);
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
    };
}

