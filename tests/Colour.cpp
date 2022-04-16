#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "splash/Splash.hpp"

namespace {
    ::testing::AssertionResult IsBetween(int val, int a, int b) {
        if (a > b) {
            return ::testing::AssertionFailure() << "Faulty test, " << a << " <= " << val << " <= " << b << " is not satisfiable";
        }
        if (val < a) {
            return ::testing::AssertionFailure() << val << " is less than " << a;
        }
        if (val > b) {
            return ::testing::AssertionFailure() << val << " is more then " << b;
        }
        return ::testing::AssertionSuccess();
    }

    TEST(ColourTest, DefaultIsBlack) {
        Splash::Colour colour = Splash::Colour();
        EXPECT_EQ(colour.raw(), (0x00000000)) << "Colour by default should be black!";
    }

    // Unless something in the algorythm explicitly requires this, I'll probably change it to RGBA in the future
    TEST(ColourTest, RawValueIsInARGB) {
        Splash::Colour colour = Splash::Colour(0xdd, 0xaa, 0x55, 0x12);
        EXPECT_EQ(colour.raw(), (0xddaa5512)) << "Colour.raw() should return an ARGB format!\n";
    }

    TEST(ColourTest, ReturnsCurrectComponents) {
        Splash::Colour colour = Splash::Colour(0x22, 0x77, 0x81, 0x18);

        EXPECT_EQ(colour.a(), 0x22);
        EXPECT_EQ(colour.r(), 0x77);
        EXPECT_EQ(colour.g(), 0x81);
        EXPECT_EQ(colour.b(), 0x18);
    }

    TEST(HslTest, Red) {
        Splash::HSL hsl = Splash::Colour(0, 255, 0, 0).hsl();
        EXPECT_EQ(hsl.h, 0.0f);
        EXPECT_EQ(hsl.s, 1.0f);
        EXPECT_EQ(hsl.l, 0.5f);
    }

    TEST(HslTest, Green) {
        Splash::HSL hsl = Splash::Colour(0, 0, 255, 0).hsl();
        EXPECT_EQ(hsl.h, 120.0f);
        EXPECT_EQ(hsl.s, 1.0f);
        EXPECT_EQ(hsl.l, 0.5f);
    }

    TEST(HslTest, Blue) {
        Splash::HSL hsl = Splash::Colour(0, 0, 0, 255).hsl();
        EXPECT_EQ(hsl.h, 240.0f);
        EXPECT_EQ(hsl.s, 1.0f);
        EXPECT_EQ(hsl.l, 0.5f);
    }

    TEST(HslTest, SomewhatOrange) {
        Splash::HSL hsl = Splash::Colour(0, 200, 105, 20).hsl(); // #c85914

        EXPECT_TRUE(IsBetween(hsl.h, 27.0f, 29.0f));
        EXPECT_TRUE(IsBetween(hsl.s, 0.81f, 0.82f));
        EXPECT_TRUE(IsBetween(hsl.l, 0.43f, 0.44f));
    }

    TEST(HslTest, OtherBlue) {
        Splash::HSL hsl = Splash::Colour(0, 10, 10, 200).hsl(); // #0a0ac8

        EXPECT_TRUE(IsBetween(hsl.h, 239.0f, 241.0f));
        EXPECT_TRUE(IsBetween(hsl.s, 0.90f, 0.91f));
        EXPECT_TRUE(IsBetween(hsl.l, 0.41f, 0.42f));
    }

    TEST(HslTest, WeirdWhite) {
        Splash::HSL hsl = Splash::Colour(0, 240, 233, 198).hsl(); // #f0e9c6

        EXPECT_TRUE(IsBetween(hsl.h, 49.0f, 51.0f));
        EXPECT_TRUE(IsBetween(hsl.s, 0.58f, 0.59f));
        EXPECT_TRUE(IsBetween(hsl.l, 0.85f, 0.86f));
    }

    TEST(ColourTest, CorrectlySetsEachComponent) {
        Splash::Colour colour = Splash::Colour(0, 0, 0, 0);

        colour.setA(100);
        EXPECT_EQ(colour.a(), 100);

        colour.setR(219);
        EXPECT_EQ(colour.r(), 219);

        colour.setG(12);
        EXPECT_EQ(colour.g(), 12);

        colour.setB(255);
        EXPECT_EQ(colour.b(), 255);
    }


    TEST(ColourTest, UpdatesRawValueCorrectly) {
        Splash::Colour colour = Splash::Colour(255, 255, 255, 255);
        EXPECT_EQ(colour.raw(), 0xffffffff);

        colour.setRaw(0xabcdef12);
        EXPECT_EQ(colour.raw(), 0xabcdef12);
    }
};
