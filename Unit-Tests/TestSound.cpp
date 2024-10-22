#include "CppUnitTest.h"

#include <string>
#include <sstream>
#include <locale>
#include <stdexcept>

#include "../Deadzone/source/ResourceManager/ResourceManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestSound
{
    TEST_CLASS(TestSound)
    {
    public:
        TEST_METHOD(walking)
        {
            Logger::WriteMessage("walking");
            TestSoundFunc("../../../Deadzone/resources/sounds/walking.mp3", FMOD_LOOP_NORMAL, "running", 59977u);
        }

        TEST_METHOD(running)
        {
            Logger::WriteMessage("running");
            TestSoundFunc("../../../Deadzone/resources/sounds/running.mp3", FMOD_LOOP_NORMAL, "running", 5952u);
        }

        TEST_METHOD(fist)
        {
            Logger::WriteMessage("fist");
            TestSoundFunc("../../../Deadzone/resources/sounds/fist.mp3", FMOD_LOOP_NORMAL, "fist", 809u);
        }

        TEST_METHOD(revolver_01)
        {
            Logger::WriteMessage("revolver_01");
            TestSoundFunc("../../../Deadzone/resources/sounds/revolver_01.wav", FMOD_LOOP_NORMAL, "revolver_01", 3500u);
        }

        TEST_METHOD(revolverEmpty)
        {
            Logger::WriteMessage("revolverEmpty");
            TestSoundFunc("../../../Deadzone/resources/sounds/revolverEmpty.wav", FMOD_LOOP_NORMAL, "revolverEmpty", 506u);
        }

        TEST_METHOD(revolverDraw)
        {
            Logger::WriteMessage("revolverDraw");
            TestSoundFunc("../../../Deadzone/resources/sounds/revolverDraw.wav", FMOD_LOOP_NORMAL, "revolverDraw", 580u);
        }

        TEST_METHOD(revolverReload)
        {
            Logger::WriteMessage("revolverReload");
            TestSoundFunc("../../../Deadzone/resources/sounds/revolverReload.wav", FMOD_LOOP_NORMAL, "revolverReload", 2061u);
        }

        TEST_METHOD(ak47_01)
        {
            Logger::WriteMessage("ak47_01");
            TestSoundFunc("../../../Deadzone/resources/sounds/ak47_01.wav", FMOD_LOOP_NORMAL, "ak47_01", 2848u);
        }

        TEST_METHOD(ak47Draw)
        {
            Logger::WriteMessage("ak47Draw");
            TestSoundFunc("../../../Deadzone/resources/sounds/ak47Draw.wav", FMOD_LOOP_NORMAL, "ak47Draw", 451u);
        }

        TEST_METHOD(ak47Reload)
        {
            Logger::WriteMessage("ak47Reload");
            TestSoundFunc("../../../Deadzone/resources/sounds/ak47Reload.wav", FMOD_LOOP_NORMAL, "ak47Reload", 1200u);
        }

        TEST_METHOD(m4a1_01)
        {
            Logger::WriteMessage("m4a1_01");
            TestSoundFunc("../../../Deadzone/resources/sounds/m4a1_01.wav", FMOD_LOOP_NORMAL, "m4a1_01", 3191u);
        }

        TEST_METHOD(m4a1_02)
        {
            Logger::WriteMessage("m4a1_02");
            TestSoundFunc("../../../Deadzone/resources/sounds/m4a1_02.wav", FMOD_LOOP_NORMAL, "m4a1_02", 1859u);
        }

        TEST_METHOD(m4a1_03)
        {
            Logger::WriteMessage("m4a1_03");
            TestSoundFunc("../../../Deadzone/resources/sounds/m4a1_03.wav", FMOD_LOOP_NORMAL, "m4a1_03", 2479u);
        }

        TEST_METHOD(m4a1_04)
        {
            Logger::WriteMessage("m4a1_04");
            TestSoundFunc("../../../Deadzone/resources/sounds/m4a1_04.wav", FMOD_LOOP_NORMAL, "m4a1_04", 2815u);
        }

        TEST_METHOD(m4a1Empty)
        {
            Logger::WriteMessage("m4a1Empty");
            TestSoundFunc("../../../Deadzone/resources/sounds/m4a1Empty.wav", FMOD_LOOP_NORMAL, "m4a1Empty", 206u);
        }

        TEST_METHOD(m4a1Draw)
        {
            Logger::WriteMessage("m4a1Draw");
            TestSoundFunc("../../../Deadzone/resources/sounds/m4a1Draw.wav", FMOD_LOOP_NORMAL, "m4a1Draw", 1023u);
        }

        TEST_METHOD(m4a1Reload)
        {
            Logger::WriteMessage("m4a1Reload");
            TestSoundFunc("../../../Deadzone/resources/sounds/m4a1Reload.wav", FMOD_LOOP_NORMAL, "m4a1Reload", 1541u);
        }

        TEST_METHOD(knifeDraw)
        {
            Logger::WriteMessage("knifeDraw");
            TestSoundFunc("../../../Deadzone/resources/sounds/knifeDraw.wav", FMOD_LOOP_NORMAL, "knifeDraw", 937u);
        }

        TEST_METHOD(knifeLook)
        {
            Logger::WriteMessage("knifeLook");
            TestSoundFunc("../../../Deadzone/resources/sounds/knifeLook.wav", FMOD_LOOP_NORMAL, "knifeLook", 902u);
        }

        TEST_METHOD(knife_01)
        {
            Logger::WriteMessage("knife_01");
            TestSoundFunc("../../../Deadzone/resources/sounds/knife_01.wav", FMOD_LOOP_NORMAL, "knife_01", 1031u);
        }

        TEST_METHOD(knife_02)
        {
            Logger::WriteMessage("knife_02");
            TestSoundFunc("../../../Deadzone/resources/sounds/knife_02.wav", FMOD_LOOP_NORMAL, "knife_02", 1031u);
        }

        TEST_METHOD(knife_03)
        {
            Logger::WriteMessage("knife_03");
            TestSoundFunc("../../../Deadzone/resources/sounds/knife_03.wav", FMOD_LOOP_NORMAL, "knife_03", 1070u);
        }

        TEST_METHOD(knife_04)
        {
            Logger::WriteMessage("knife_04");
            TestSoundFunc("../../../Deadzone/resources/sounds/knife_04.wav", FMOD_LOOP_NORMAL, "knife_04", 1828u);
        }

        TEST_METHOD(shotgun_01)
        {
            Logger::WriteMessage("shotgun_01");
            TestSoundFunc("../../../Deadzone/resources/sounds/shotgun_01.wav", FMOD_LOOP_NORMAL, "shotgun_01", 1046u);
        }

        TEST_METHOD(shotgunDraw)
        {
            Logger::WriteMessage("shotgunDraw");
            TestSoundFunc("../../../Deadzone/resources/sounds/shotgunDraw.wav", FMOD_LOOP_NORMAL, "shotgunDraw", 736u);
        }

        TEST_METHOD(shotgunReload)
        {
            Logger::WriteMessage("shotgunReload");
            TestSoundFunc("../../../Deadzone/resources/sounds/shotgunReload.wav", FMOD_LOOP_NORMAL, "shotgunReload", 2195u);
        }

        TEST_METHOD(shotgunEmpty)
        {
            Logger::WriteMessage("shotgunEmpty");
            TestSoundFunc("../../../Deadzone/resources/sounds/shotgunEmpty.wav", FMOD_LOOP_NORMAL, "shotgunEmpty", 462u);
        }

        TEST_METHOD(grenadeDetonate_01)
        {
            Logger::WriteMessage("grenadeDetonate_01");
            TestSoundFunc("../../../Deadzone/resources/sounds/grenadeDetonate_01.wav", FMOD_LOOP_NORMAL, "grenadeDetonate_01", 4743u);
        }

        TEST_METHOD(grenadeDetonate_02)
        {
            Logger::WriteMessage("grenadeDetonate_02");
            TestSoundFunc("../../../Deadzone/resources/sounds/grenadeDetonate_02.wav", FMOD_LOOP_NORMAL, "grenadeDetonate_02", 4863u);
        }

        TEST_METHOD(grenadeDetonate_03)
        {
            Logger::WriteMessage("grenadeDetonate_03");
            TestSoundFunc("../../../Deadzone/resources/sounds/grenadeDetonate_03.wav", FMOD_LOOP_NORMAL, "grenadeDetonate_03", 4627u);
        }

        TEST_METHOD(grenadeDraw)
        {
            Logger::WriteMessage("grenadeDraw");
            TestSoundFunc("../../../Deadzone/resources/sounds/grenadeDraw.wav", FMOD_LOOP_NORMAL, "grenadeDraw", 437u);
        }

        TEST_METHOD(grenadeThrow_01)
        {
            Logger::WriteMessage("grenadeThrow_01");
            TestSoundFunc("../../../Deadzone/resources/sounds/grenadeThrow_01.wav", FMOD_LOOP_NORMAL, "grenadeThrow_01", 184u);
        }

        TEST_METHOD(grenadeThrow_02)
        {
            Logger::WriteMessage("grenadeThrow_02");
            TestSoundFunc("../../../Deadzone/resources/sounds/grenadeThrow_02.wav", FMOD_LOOP_NORMAL, "grenadeThrow_02", 166u);
        }

        TEST_METHOD(newWave)
        {
            Logger::WriteMessage("grenadeThrow_02");
            TestSoundFunc("../../../Deadzone/resources/sounds/newWave.mp3", FMOD_LOOP_NORMAL, "newWave", 11592u);
        }

        TEST_METHOD(soundtrack)
        {
            Logger::WriteMessage("soundtrack");
            TestSoundFunc("../../../Deadzone/resources/sounds/soundtrack.mp3", FMOD_LOOP_NORMAL, "soundtrack", 185887u);
        }

        TEST_METHOD(door)
        {
            Logger::WriteMessage("door");
            TestSoundFunc("../../../Deadzone/resources/sounds/door.mp3", FMOD_LOOP_NORMAL, "door", 2115u);
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

        void TestSoundFunc(const std::string& resourcePath, int mode, const std::string& soundName, unsigned int soundLength)
        {
            try
            {
                ResourceManager::loadSound(resourcePath.c_str(), mode, soundName);
            }
            catch (const std::runtime_error& err)
            {
                Assert::Fail(widen(err.what()).c_str());
            }
            catch (...)
            {
                Assert::Fail(L"Other errors");
            }

            unsigned int length;
            ResourceManager::getSound(soundName)->getLength(&length, FMOD_TIMEUNIT_MS);
            Assert::AreEqual(soundLength, length);
        }
    };
}

