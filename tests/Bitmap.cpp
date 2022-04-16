#include <array>
#include <tuple>
#include <gtest/gtest.h>
#include "splash/Splash.hpp"

namespace {
    TEST(BitmapTest, DefaultBitmapIsInvalid) {
        Splash::Bitmap bitmap;
        EXPECT_FALSE(bitmap.isValid()) << "A default bitmap should always be invalid!";

        bitmap.setWidth(100);
        bitmap.setHeight(100);
        EXPECT_FALSE(bitmap.isValid()) << "A default bitmap should always be invalid, even after resizing!";
    }

    TEST(BitmapTest, BitmapSizeIsCorrectAfterInit) {
        Splash::Bitmap bitmap = Splash::Bitmap(55, 123);
        EXPECT_EQ(bitmap.getWidth(), 55) << "A non-resized bitmap should be the same width as when inited!";
        EXPECT_EQ(bitmap.getHeight(), 123) << "A non-resized bitmap should be the same height as when inited!";
    }

    TEST(BitmapTest, InitedBitmapIsWhite) {
        Splash::Bitmap bitmap = Splash::Bitmap(50, 50);

        bool good = true;
        std::vector<Splash::Colour> pixels = bitmap.getPixels(0, 0, 50, 50);
        for (size_t i = 0; i < pixels.size(); i++) {
            if (pixels[i].raw() != (0xffffffff)) {
                good = false;
                break;
            }
        }
        EXPECT_TRUE(good) << "After init, the bitmap should contain only white pixels!";
    }

    // TODO: This is dumb, we should return an error
    TEST(BitmapTest, ReadingSinglePixelOutOfBoundsGivesWhitePixel) {
        Splash::Bitmap bitmap = Splash::Bitmap(10, 10);

        EXPECT_EQ(bitmap.getPixel(20, 20).raw(), 0xffffffff) << "Out of bounds pixels should be white!";
    }

    // I still think they should return errors...
    TEST(BitmapTest, ReadingMultiplePixelsOutOfBoundsGivesOnlyValidPixelsBack) {
        Splash::Bitmap bitmap = Splash::Bitmap(10, 10);

        std::vector<Splash::Colour> pixels = bitmap.getPixels(8, 8, 10, 10);
        EXPECT_EQ(pixels.size(), 4) << "Reading partially out of bounds should return only valid pixels!";

        pixels = bitmap.getPixels(100, 50, 230, 23);
        EXPECT_EQ(pixels.size(), 0) << "Reading completely out of bounds should return no pixels!";
    }

    TEST(BitmapTest, PixelsAreReadableAndWriteable) {
        Splash::Bitmap bitmap = Splash::Bitmap(20, 20);
        Splash::Colour colour = Splash::Colour(10, 255, 0, 20);

        bitmap.setPixel(colour, 15, 15);
        EXPECT_EQ(bitmap.getPixel(15, 15).raw(), 0x0aff0014) << "The colour of a pixel should be writeable!";

        bitmap.setPixel(colour, 1000, 230);
        EXPECT_EQ(bitmap.getPixel(1000, 230).raw(), 0xffffffff) << "Writing out of bounds pixels should keep them white!";
    }

    TEST(BitmapTest, ResizesCorrectly) {
        Splash::Bitmap bitmap = Splash::Bitmap(30, 100);
        bitmap.setWidth(1000);
        EXPECT_EQ(bitmap.getWidth(), 1000) << "Width should be resizeable!";
        EXPECT_EQ(bitmap.getHeight(), 100) << "Changing width should not change height!";

        bitmap.setHeight(565);
        EXPECT_EQ(bitmap.getHeight(), 565) << "Height should be resizeable!";
        EXPECT_EQ(bitmap.getWidth(), 1000) << "Changing height should not change width!";
    }

    std::tuple<Splash::Bitmap, std::array<Splash::Colour, 4>> setupForScalingTests() {
        std::array<Splash::Colour, 4> colours = {
            Splash::Colour(0, 255, 255, 0),
            Splash::Colour(0, 0, 255, 30),
            Splash::Colour(0, 255, 40, 255),
            Splash::Colour(0, 25, 55, 200),
        };

        // We'll use a simple 4 colour bitmap here
        Splash::Bitmap bitmap = Splash::Bitmap(4, 4);
        std::vector<Splash::Colour> pixels;
        for (size_t y = 0; y < 4; y++) {
            for (size_t x = 0; x < 4; x++) {
                size_t idx = (x / 2 + (2 * (y / 2)));
                pixels.push_back(colours[idx]);
            }
        }
        bitmap.setPixels(pixels, 0, 0, 4, 4);

        return {bitmap, colours};
    }

    TEST(BitmapTest, ScalesDownCorrectly) {
        auto [bitmap, colours] = setupForScalingTests();

        Splash::Bitmap scaledBitmap = bitmap.createScaledBitmap(2, 2);
        std::vector<Splash::Colour> cols = scaledBitmap.getPixels(0, 0, 2, 2);

        ASSERT_EQ(cols[0].raw(), colours[0].raw());
        ASSERT_EQ(cols[1].raw(), colours[1].raw());
        ASSERT_EQ(cols[2].raw(), colours[2].raw());
        ASSERT_EQ(cols[3].raw(), colours[3].raw());
    }

    TEST(BitmapTest, ScapesUpCorrectly) {
        auto [bitmap, colours] = setupForScalingTests();

        Splash::Bitmap scaledBitmap = bitmap.createScaledBitmap(8, 8);
        std::vector<Splash::Colour> cols = scaledBitmap.getPixels(0, 0, 8, 8);

        for (size_t y = 0; y < 8; y++) {
            for (size_t x = 0; x < 8; x++) {
                size_t idx = (x / 4 + (2 * (y / 4)));
                size_t idy = (y * 8) + x;
                // Assert on purpose, so we quit early at the first error
                ASSERT_EQ(cols[idy].raw(), colours[idx].raw()) << "Scaling up should not change the colours of pixels!";
            }
        }
    }
};
