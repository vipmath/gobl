// goblr_matrix.t.cpp

#include <goblr_matrix.h>

#include <gtest/gtest.h>

#include <chrono>
#include <sstream>
#include <fstream>

namespace goblr {

TEST(Matrix, constructor)
{
    {
        Matrix matrix;
        const unsigned int size = goblb::Board::SIZE;
        EXPECT_EQ(size, matrix.size());

        for(unsigned int i = 0; i < goblb::Board::SIZE; ++i)
        {
            for(unsigned int j = 0; j < goblb::Board::SIZE; ++j)
            {
                EXPECT_EQ(0.0, matrix.get(i, j));
            }
        }
    }
    {
        const unsigned int size = 31;

        Matrix matrix(size);
        EXPECT_EQ(size, matrix.size());

        for(unsigned int i = 0; i < size; ++i)
        {
            for(unsigned int j = 0; j < size; ++j)
            {
                EXPECT_EQ(0.0, matrix.get(i, j));
            }
        }
    }
}

TEST(Matrix, getSet)
{
    Matrix matrix;

    matrix.set(9, 3, 5.9);
    EXPECT_EQ(5.9, matrix.get(9, 3));

    matrix.set(9, 3, 6.9);
    EXPECT_EQ(6.9, matrix.get(9, 3));

    matrix.set(4, 7, -3.9);
    EXPECT_EQ(-3.9, matrix.get(4, 7));
    EXPECT_EQ(6.9, matrix.get(9, 3));
}

TEST(Matrix, print)
{
    Matrix matrix(3);

    matrix.set(0, 0, 4.0);
    matrix.set(1, 0, 4.93);
    matrix.set(0, 1, 3.456);
    matrix.set(1, 1, 9.34);

    std::ostringstream stream;
    matrix.print(stream);

    std::cout << stream.str() << std::endl;

    const char result[] = "\n 2  [ 0.000, 0.000, 0.000 ]\n"
                            " 1  [ 4.930, 9.340, 0.000 ]\n"
                            " 0  [ 4.000, 3.456, 0.000 ]\n"
                            "        0      1      2\n";

    EXPECT_STREQ(result, stream.str().c_str());
}

TEST(Matrix, equals)
{
    Matrix matrix;
    EXPECT_TRUE(matrix == matrix);
    EXPECT_FALSE(matrix != matrix);

    Matrix matrix2;
    EXPECT_TRUE(matrix == matrix2);
    EXPECT_FALSE(matrix != matrix2);

    matrix2.set(0, 3, 1.0);
    EXPECT_FALSE(matrix == matrix2);
    EXPECT_TRUE(matrix != matrix2);

    matrix.set(0, 3, 1.0);
    EXPECT_TRUE(matrix == matrix2);
    EXPECT_FALSE(matrix != matrix2);

    matrix2.set(4, 3, 7.98);
    EXPECT_FALSE(matrix == matrix2);
    EXPECT_TRUE(matrix != matrix2);
}

TEST(Matrix, writeReadFromFile)
{
    std::time_t now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );
    std::ostringstream sstream;
    sstream << ".goblr.test.temp" << now;
    const std::string tempFileName = sstream.str();

    std::cout << "Using file: " << tempFileName << std::endl;

    {
        Matrix matrix;
        for(unsigned int i = 0; i < goblb::Board::SIZE; ++i)
        {
            for(unsigned int j = 0; j < goblb::Board::SIZE; ++j)
            {
                matrix.set(i, j, i + j);
            }
        }

        std::ofstream fstream(tempFileName, std::ofstream::binary);
        matrix.writeToFile(fstream);

    }
    {
        Matrix matrix;
        std::ifstream fstream(tempFileName, std::ifstream::binary);
        matrix.readFromFile(fstream);
        for(unsigned int i = 0; i < goblb::Board::SIZE; ++i)
        {
            for(unsigned int j = 0; j < goblb::Board::SIZE; ++j)
            {
                EXPECT_EQ(matrix.get(i, j), i + j);
            }
        }
    }

    std::remove(tempFileName.c_str());
}

} // Close goblr
