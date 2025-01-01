#include "FlyFish.h"

// Type conversions

[[nodiscard]] TwoBlade Motor::Grade2() const
{
    return TwoBlade(
        data[1], data[2], data[3], data[4], data[5], data[6]
    );
}
[[nodiscard]] OneBlade MultiVector::Grade1() const
{
    return OneBlade{
        data[1],
        data[2],
        data[3],
        data[4]
    };
}
[[nodiscard]] TwoBlade MultiVector::Grade2() const
{
    return TwoBlade{
        data[5],
        data[6],
        data[7],
        data[8],
        data[9],
        data[10]
    };
}
[[nodiscard]] ThreeBlade MultiVector::Grade3() const
{
    return ThreeBlade{
        data[11],
        data[12],
        data[13],
        data[14],
    };
}
[[nodiscard]] Motor MultiVector::ToMotor() const
{
    return Motor{
        data[0],
        data[5],
        data[6],
        data[7],
        data[8],
        data[9],
        data[10],
        data[15]
    };
}

// Copy/move assignments

MultiVector& MultiVector::operator=(const ThreeBlade& b)
{
    data.fill(0);
    data[11] = b[0];
    data[12] = b[1];
    data[13] = b[2];
    data[14] = b[3];
    return *this;
}
MultiVector& MultiVector::operator=(ThreeBlade&& b) noexcept
{
    data.fill(0);
    data[11] = b[0];
    data[12] = b[1];
    data[13] = b[2];
    data[14] = b[3];
    return *this;
};
MultiVector& MultiVector::operator=(const TwoBlade& b)
{
    data.fill(0);
    data[5] = b[0];
    data[6] = b[1];
    data[7] = b[2];
    data[8] = b[3];
    data[9] = b[4];
    data[10] = b[5];
    return *this;
}
MultiVector& MultiVector::operator=(TwoBlade&& b) noexcept
{
    data.fill(0);
    data[5] = b[0];
    data[6] = b[1];
    data[7] = b[2];
    data[8] = b[3];
    data[9] = b[4];
    data[10] = b[5];
    return *this;
};
MultiVector& MultiVector::operator=(const OneBlade& b)
{
    data.fill(0);
    data[1] = b[0];
    data[2] = b[1];
    data[3] = b[2];
    data[4] = b[3];
    return *this;
}
MultiVector& MultiVector::operator=(OneBlade&& b) noexcept
{
    data.fill(0);
    data[1] = b[0];
    data[2] = b[1];
    data[3] = b[2];
    data[4] = b[3];
    return *this;
};
MultiVector& MultiVector::operator=(const Motor& b)
{
    data.fill(0);
    data[0] = b[0];
    data[5] = b[1];
    data[6] = b[2];
    data[7] = b[3];
    data[8] = b[4];
    data[9] = b[5];
    data[10] = b[6];
    data[15] = b[7];
    return *this;
}
MultiVector& MultiVector::operator=(Motor&& b) noexcept
{
    data.fill(0);
    data[0] = b[0];
    data[5] = b[1];
    data[6] = b[2];
    data[7] = b[3];
    data[8] = b[4];
    data[9] = b[5];
    data[10] = b[6];
    data[15] = b[7];
    return *this;
};

// Geometric Product

// MultiVector
[[nodiscard]] MultiVector MultiVector::operator* (const MultiVector& b) const {
    MultiVector res{};
    res[0] = b[0] * data[0] + b[2] * data[2] + b[3] * data[3] + b[4] * data[4] - b[10] * data[10] - b[9] * data[9] - b[8] * data[8] - b[14] * data[14];
    res[1] = b[1] * data[0] + b[0] * data[1] - b[5] * data[2] - b[6] * data[3] - b[7] * data[4] + b[2] * data[5] + b[3] * data[6] + b[4] * data[7] + b[13] * data[10] + b[12] * data[9] + b[11] * data[8] + b[10] * data[13] + b[9] * data[12] + b[8] * data[11] + b[15] * data[14] - b[14] * data[15];
    res[2] = b[2] * data[0] + b[0] * data[2] - b[10] * data[3] + b[9] * data[4] + b[3] * data[10] - b[4] * data[9] - b[14] * data[8] - b[8] * data[14];
    res[3] = b[3] * data[0] + b[10] * data[2] + b[0] * data[3] - b[8] * data[4] - b[2] * data[10] - b[14] * data[9] + b[4] * data[8] - b[9] * data[14];
    res[4] = b[4] * data[0] - b[9] * data[2] + b[8] * data[3] + b[0] * data[4] - b[14] * data[10] + b[2] * data[9] - b[3] * data[8] - b[10] * data[14];
    res[5] = b[5] * data[0] + b[2] * data[1] - b[1] * data[2] - b[13] * data[3] + b[12] * data[4] + b[0] * data[5] - b[10] * data[6] + b[9] * data[7] + b[6] * data[10] - b[7] * data[9] - b[15] * data[8] - b[3] * data[13] + b[4] * data[12] + b[14] * data[11] - b[11] * data[14] - b[8] * data[15];
    res[6] = b[6] * data[0] + b[3] * data[1] + b[13] * data[2] - b[1] * data[3] - b[11] * data[4] + b[10] * data[5] + b[0] * data[6] - b[8] * data[7] - b[5] * data[10] - b[15] * data[9] + b[7] * data[8] + b[2] * data[13] + b[14] * data[12] - b[4] * data[11] - b[12] * data[14] - b[9] * data[15];
    res[7] = b[7] * data[0] + b[4] * data[1] - b[12] * data[2] + b[11] * data[3] - b[1] * data[4] - b[9] * data[5] + b[8] * data[6] + b[0] * data[7] - b[15] * data[10] + b[5] * data[9] - b[6] * data[8] + b[14] * data[13] - b[2] * data[12] + b[3] * data[11] - b[13] * data[14] - b[10] * data[15];
    res[8] = b[8] * data[0] + b[14] * data[2] + b[4] * data[3] - b[3] * data[4] + b[9] * data[10] - b[10] * data[9] + b[0] * data[8] + b[2] * data[14];
    res[9] = b[9] * data[0] - b[4] * data[2] + b[14] * data[3] + b[2] * data[4] - b[8] * data[10] + b[0] * data[9] + b[10] * data[8] + b[3] * data[14];
    res[10] = b[10] * data[0] + b[3] * data[2] - b[2] * data[3] + b[14] * data[4] + b[0] * data[10] + b[8] * data[9] - b[9] * data[8] + b[4] * data[14];
    res[11] = b[11] * data[0] - b[8] * data[1] + b[15] * data[2] + b[7] * data[3] - b[6] * data[4] - b[14] * data[5] - b[4] * data[6] + b[3] * data[7] + b[12] * data[10] - b[13] * data[9] - b[1] * data[8] + b[9] * data[13] - b[10] * data[12] + b[0] * data[11] + b[5] * data[14] - b[2] * data[15];
    res[12] = b[12] * data[0] - b[9] * data[1] - b[7] * data[2] + b[15] * data[3] + b[5] * data[4] + b[4] * data[5] - b[14] * data[6] - b[2] * data[7] - b[11] * data[10] - b[1] * data[9] + b[13] * data[8] - b[8] * data[13] + b[0] * data[12] + b[10] * data[11] + b[6] * data[14] - b[3] * data[15];
    res[13] = b[13] * data[0] - b[10] * data[1] + b[6] * data[2] - b[5] * data[3] + b[15] * data[4] - b[3] * data[5] + b[2] * data[6] - b[14] * data[7] - b[1] * data[10] + b[11] * data[9] - b[12] * data[8] + b[0] * data[13] + b[8] * data[12] - b[9] * data[11] + b[7] * data[14] - b[4] * data[15];
    res[14] = b[14] * data[0] + b[8] * data[2] + b[9] * data[3] + b[10] * data[4] + b[4] * data[10] + b[3] * data[9] + b[2] * data[8] + b[0] * data[14];
    res[15] = b[15] * data[0] + b[14] * data[1] + b[11] * data[2] + b[12] * data[3] + b[13] * data[4] + b[8] * data[5] + b[9] * data[6] + b[10] * data[7] + b[7] * data[10] + b[6] * data[9] + b[5] * data[8] - b[4] * data[13] - b[3] * data[12] - b[2] * data[11] - b[1] * data[14] + b[0] * data[15];
    return res;
};
[[nodiscard]] MultiVector MultiVector::operator* (const ThreeBlade& b) const
{
    MultiVector res{};
    res[0] = - b[3] * data[14];
    res[1] = b[2] * data[10] + b[1] * data[9] + b[0] * data[8] - b[3] * data[15];
    res[2] = - b[3] * data[8];
    res[3] = - b[3] * data[9];
    res[4] = - b[3] * data[10];
    res[5] = - b[2] * data[3] + b[1] * data[4] + b[3] * data[11] - b[0] * data[14];
    res[6] = b[2] * data[2] - b[0] * data[4] + b[3] * data[12] - b[1] * data[14];
    res[7] = - b[1] * data[2] + b[0] * data[3] + b[3] * data[13] - b[2] * data[14];
    res[8] = b[3] * data[2];
    res[9] = b[3] * data[3];
    res[10] = b[3] * data[4];
    res[11] = b[0] * data[0] - b[3] * data[5] + b[1] * data[10] - b[2] * data[9];
    res[12] = b[1] * data[0] - b[3] * data[6] - b[0] * data[10] + b[2] * data[8];
    res[13] = b[2] * data[0] - b[3] * data[7] + b[0] * data[9] - b[1] * data[8];
    res[14] = b[3] * data[0];
    res[15] = b[3] * data[1] + b[0] * data[2] + b[1] * data[3] + b[2] * data[4];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator* (const TwoBlade& b) const {
    MultiVector res{};
    res[0] = -b[5] * data[10] - b[4] * data[9] - b[3] * data[8];
    res[1] = -b[0] * data[2] - b[1] * data[3] - b[2] * data[4] + b[5] * data[13] + b[4] * data[12] + b[3] * data[11];
    res[2] = -b[5] * data[3] + b[4] * data[4] - b[3] * data[14];
    res[3] = b[5] * data[2] - b[3] * data[4] - b[4] * data[14];
    res[4] = -b[4] * data[2] + b[3] * data[3] - b[5] * data[14];
    res[5] = b[0] * data[0] - b[5] * data[6] + b[4] * data[7] + b[1] * data[10] - b[2] * data[9] - b[3] * data[15];
    res[6] = b[1] * data[0] + b[5] * data[5] - b[3] * data[7] - b[0] * data[10] + b[2] * data[8] - b[4] * data[15];
    res[7] = b[2] * data[0] - b[4] * data[5] + b[3] * data[6] + b[0] * data[9] - b[1] * data[8] - b[5] * data[15];
    res[8] = b[3] * data[0] + b[4] * data[10] - b[5] * data[9];
    res[9] = b[4] * data[0] - b[3] * data[10] + b[5] * data[8];
    res[10] = b[5] * data[0] + b[3] * data[9] - b[4] * data[8];
    res[11] = -b[3] * data[1] + b[2] * data[3] - b[1] * data[4] + b[4] * data[13] - b[5] * data[12] + b[0] * data[14];
    res[12] = -b[4] * data[1] - b[2] * data[2] + b[0] * data[4] - b[3] * data[13] + b[5] * data[11] + b[1] * data[14];
    res[13] = -b[5] * data[1] + b[1] * data[2] - b[0] * data[3] + b[3] * data[12] - b[4] * data[11] + b[2] * data[14];
    res[14] = b[3] * data[2] + b[4] * data[3] + b[5] * data[4];
    res[15] = b[3] * data[5] + b[4] * data[6] + b[5] * data[7] + b[2] * data[10] + b[1] * data[9] + b[0] * data[8];
    return res;
};
[[nodiscard]] MultiVector MultiVector::operator* (const OneBlade& b) const
{
    MultiVector res{};
    res[0] = b[1] * data[2] + b[2] * data[3] + b[3] * data[4];
    res[1] = b[0] * data[0] + b[1] * data[5] + b[2] * data[6] + b[3] * data[7];
    res[2] = b[1] * data[0] + b[2] * data[10] - b[3] * data[9];
    res[3] = b[2] * data[0] - b[1] * data[10] + b[3] * data[8];
    res[4] = b[3] * data[0] + b[1] * data[9] - b[2] * data[8];
    res[5] = b[1] * data[1] - b[0] * data[2] - b[2] * data[13] + b[3] * data[12];
    res[6] = b[2] * data[1] - b[0] * data[3] + b[1] * data[13] - b[3] * data[11];
    res[7] = b[3] * data[1] - b[0] * data[4] - b[1] * data[12] + b[2] * data[11];
    res[8] = b[3] * data[3] - b[2] * data[4] + b[1] * data[14];
    res[9] = - b[3] * data[2] + b[1] * data[4] + b[2] * data[14];
    res[10] = b[2] * data[2] - b[1] * data[3] + b[3] * data[14];
    res[11] = - b[3] * data[6] + b[2] * data[7] - b[0] * data[8] - b[1] * data[15];
    res[12] = b[3] * data[5] - b[1] * data[7] - b[0] * data[9] - b[2] * data[15];
    res[13] = - b[2] * data[5] + b[1] * data[6] - b[0] * data[10] - b[3] * data[15];
    res[14] = b[3] * data[10] + b[2] * data[9] + b[1] * data[8];
    res[15] = - b[3] * data[13] - b[2] * data[12] - b[1] * data[11] - b[0] * data[14];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator* (const Motor& b) const
{
    MultiVector res{};
    res[0] = b[0] * data[0] - b[6] * data[10] - b[5] * data[9] - b[4] * data[8];
    res[1] = b[0] * data[1] - b[1] * data[2] - b[2] * data[3] - b[3] * data[4] + b[6] * data[13] + b[5] * data[12] + b[4] * data[11] + b[7] * data[14];
    res[2] = b[0] * data[2] - b[6] * data[3] + b[5] * data[4] - b[4] * data[14];
    res[3] = b[6] * data[2] + b[0] * data[3] - b[4] * data[4] - b[5] * data[14];
    res[4] = -b[5] * data[2] + b[4] * data[3] + b[0] * data[4] - b[6] * data[14];
    res[5] = b[1] * data[0] + b[0] * data[5] - b[6] * data[6] + b[5] * data[7] + b[2] * data[10] - b[3] * data[9] - b[7] * data[8] - b[4] * data[15];
    res[6] = b[2] * data[0] + b[6] * data[5] + b[0] * data[6] - b[4] * data[7] - b[1] * data[10] - b[7] * data[9] + b[3] * data[8] - b[5] * data[15];
    res[7] = b[3] * data[0] - b[5] * data[5] + b[4] * data[6] + b[0] * data[7] - b[7] * data[10] + b[1] * data[9] - b[2] * data[8] - b[6] * data[15];
    res[8] = b[4] * data[0] + b[5] * data[10] - b[6] * data[9] + b[0] * data[8];
    res[9] = b[5] * data[0] - b[4] * data[10] + b[0] * data[9] + b[6] * data[8];
    res[10] = b[6] * data[0] + b[0] * data[10] + b[4] * data[9] - b[5] * data[8];
    res[11] = -b[4] * data[1] + b[7] * data[2] + b[3] * data[3] - b[2] * data[4] + b[5] * data[13] - b[6] * data[12] + b[0] * data[11] + b[1] * data[14];
    res[12] = -b[5] * data[1] - b[3] * data[2] + b[7] * data[3] + b[1] * data[4] - b[4] * data[13] + b[0] * data[12] + b[6] * data[11] + b[2] * data[14];
    res[13] = -b[6] * data[1] + b[2] * data[2] - b[1] * data[3] + b[7] * data[4] + b[0] * data[13] + b[4] * data[12] - b[5] * data[11] + b[3] * data[14];
    res[14] = b[4] * data[2] + b[5] * data[3] + b[6] * data[4] + b[0] * data[14];
    res[15] = b[7] * data[0] + b[4] * data[5] + b[5] * data[6] + b[6] * data[7] + b[3] * data[10] + b[2] * data[9] + b[1] * data[8] + b[0] * data[15];
    return res;
}
// ThreeBlade
[[nodiscard]] MultiVector ThreeBlade::operator* (const MultiVector& b) const
{
    MultiVector res{};
    res[0] = - b[14] * data[3];
    res[1] = b[10] * data[2] + b[9] * data[1] + b[8] * data[0] + b[15] * data[3];
    res[2] = - b[8] * data[3];
    res[3] = - b[9] * data[3];
    res[4] = - b[10] * data[3];
    res[5] = - b[3] * data[2] + b[4] * data[1] + b[14] * data[0] - b[11] * data[3];
    res[6] = b[2] * data[2] + b[14] * data[1] - b[4] * data[0] - b[12] * data[3];
    res[7] = b[14] * data[2] - b[2] * data[1] + b[3] * data[0] - b[13] * data[3];
    res[8] = b[2] * data[3];
    res[9] = b[3] * data[3];
    res[10] = b[4] * data[3];
    res[11] = b[9] * data[2] - b[10] * data[1] + b[0] * data[0] + b[5] * data[3];
    res[12] = - b[8] * data[2] + b[0] * data[1] + b[10] * data[0] + b[6] * data[3];
    res[13] = b[0] * data[2] + b[8] * data[1] - b[9] * data[0] + b[7] * data[3];
    res[14] = b[0] * data[3];
    res[15] = - b[4] * data[2] - b[3] * data[1] - b[2] * data[0] - b[1] * data[3];
    return res;
}
[[nodiscard]] Motor ThreeBlade::operator* (const ThreeBlade& b) const
{
    Motor res{};
    res[0] = -b[3] * data[3];
    res[1] = b[3] * data[0] - b[0] * data[3];
    res[2] = b[3] * data[1] - b[1] * data[3];
    res[3] = b[3] * data[2] - b[2] * data[3];
    res[4] = 0;
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    return res;
}
[[nodiscard]] MultiVector ThreeBlade::operator* (const TwoBlade& b) const {
    MultiVector res{};
    res[0] = 0;
    res[1] = b[5] * data[2] + b[4] * data[1] + b[3] * data[0];
    res[2] = -b[3] * data[3];
    res[3] = -b[4] * data[3];
    res[4] = -b[5] * data[3];
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[11] = b[4] * data[2] - b[5] * data[1] + b[1] * data[3];
    res[12] = -b[3] * data[2] + b[5] * data[0] + b[1] * data[3];
    res[13] = b[3] * data[1] - b[4] * data[0] + b[2] * data[3];
    res[14] = 0;
    res[15] = 0;
    return res;
};
[[nodiscard]] Motor ThreeBlade::operator* (const OneBlade& b) const
{
    Motor res{};
    res[0] = 0;
    res[1] = -b[2] * data[2] + b[3] * data[1];
    res[2] = b[1] * data[2] - b[3] * data[0];
    res[3] = - b[1] * data[1] + b[2] * data[0];
    res[4] = b[1] * data[3];
    res[5] = b[2] * data[3];
    res[6] = b[3] * data[3];
    res[7] = -b[3] * data[2] - b[2] * data[1] - b[1] * data[0] - b[0] * data[3];
    return res;
}
[[nodiscard]] MultiVector ThreeBlade::operator* (const Motor& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = b[6] * data[2] + b[5] * data[1] + b[4] * data[0] + b[7] * data[3];
    res[2] = -b[4] * data[3];
    res[3] = -b[5] * data[3];
    res[4] = -b[6] * data[3];
    res[6] = 0;
    res[7] = 0;
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[11] = b[5] * data[2] - b[6] * data[1] + b[0] * data[0] + b[1] * data[3];
    res[12] = -b[4] * data[2] + b[0] * data[1] + b[6] * data[0] + b[2] * data[3];
    res[13] = b[0] * data[2] + b[4] * data[1] - b[5] * data[0] + b[3] * data[3];
    res[14] = b[0] * data[3];
    res[15] = 0;
    return res;
}
// TwoBlade
[[nodiscard]] MultiVector TwoBlade::operator* (const MultiVector& b) const {
    MultiVector res{};
    res[0] = - b[10] * data[5] - b[9] * data[4] - b[8] * data[3];
    res[1] = b[2] * data[0] + b[3] * data[1] + b[4] * data[2] + b[13] * data[5] + b[12] * data[4] + b[11] * data[3];
    res[2] = b[3] * data[5] - b[4] * data[4] - b[14] * data[3];
    res[3] = - b[2] * data[5] - b[14] * data[4] + b[4] * data[3];
    res[4] =- b[14] * data[5] + b[2] * data[4] - b[3] * data[3];
    res[5] = b[0] * data[0] - b[10] * data[1] + b[9] * data[2] + b[6] * data[5] - b[7] * data[4] - b[15] * data[3];
    res[6] = b[10] * data[0] + b[0] * data[1] - b[8] * data[2] - b[5] * data[5] - b[15] * data[4] + b[7] * data[3];
    res[7] = - b[9] * data[0] + b[8] * data[1] + b[0] * data[2] - b[15] * data[5] + b[5] * data[4] - b[6] * data[3];
    res[8] = b[9] * data[5] - b[10] * data[4] + b[0] * data[3];
    res[9] = - b[8] * data[5] + b[0] * data[4] + b[10] * data[3];
    res[10] = b[0] * data[5] + b[8] * data[4] - b[9] * data[3];
    res[11] = - b[14] * data[0] - b[4] * data[1] + b[3] * data[2] + b[12] * data[5] - b[13] * data[4] - b[1] * data[3];
    res[12] = b[4] * data[0] - b[14] * data[1] - b[2] * data[2] - b[11] * data[5] - b[1] * data[4] + b[13] * data[3];
    res[13] = - b[3] * data[0] + b[2] * data[1] - b[14] * data[2] - b[1] * data[5] + b[11] * data[4] - b[12] * data[3];
    res[14] = b[4] * data[5] + b[3] * data[4] + b[2] * data[3];
    res[15] = b[8] * data[0] + b[9] * data[1] + b[10] * data[2] + b[7] * data[5] + b[6] * data[4] + b[5] * data[3];
    return res;
};
[[nodiscard]] MultiVector TwoBlade::operator* (const ThreeBlade& b) const {
    MultiVector res{};
    res[0] = 0;
    res[1] = b[2] * data[5] + b[1] * data[4] + b[0] * data[3];
    res[2] = - b[3] * data[3];
    res[3] = - b[3] * data[4];
    res[4] = -b[3] * data[5];
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[11] = -b[3] * data[0] + b[1] * data[5] - b[2] * data[4];
    res[12] = - b[3] * data[1] - b[0] * data[5] + b[2] * data[3];
    res[13] = - b[3] * data[2] + b[0] * data[4] - b[1] * data[3];
    res[14] = 0;
    res[15] = 0;
    return res;
};
[[nodiscard]] Motor TwoBlade::operator* (const TwoBlade& b) const {
    Motor res{};
    res[0] = -b[5] * data[5] - b[4] * data[4] - b[3] * data[3];
    res[1] = - b[5] * data[1] + b[4] * data[2] + b[1] * data[5] - b[2] * data[4];
    res[2] = b[5] * data[0] - b[3] * data[2] - b[0] * data[5] + b[2] * data[3];
    res[3] = -b[4] * data[0] + b[3] * data[1] + b[0] * data[4] - b[1] * data[3];
    res[4] = b[4] * data[5] - b[5] * data[4];
    res[5] = -b[3] * data[5] + b[5] * data[3];
    res[6] = b[3] * data[4] - b[4] * data[3];
    res[7] = b[3] * data[0] + b[4] * data[1] + b[5] * data[2] + b[2] * data[5] + b[1] * data[4] + b[0] * data[3];
    return res;
};
[[nodiscard]] MultiVector TwoBlade::operator* (const OneBlade& b) const {
    MultiVector res{};
    res[1] = b[1] * data[0] + b[2] * data[1] + b[3] * data[2];
    res[2] = b[2] * data[5] - b[3] * data[4];
    res[3] = -b[1] * data[5] + b[3] * data[3];
    res[4] = b[1] * data[4] - b[2] * data[3];
    res[11] = - b[3] * data[1] + b[2] * data[2] - b[0] * data[3];
    res[12] = b[3] * data[0] - b[1] * data[2] - b[0] * data[4];
    res[13] = -b[2] * data[0] + b[1] * data[1] - b[0] * data[5];
    res[14] = b[3] * data[5] + b[2] * data[4] + b[1] * data[3];
    return res;
};
[[nodiscard]] Motor TwoBlade::operator* (const Motor& b) const {
    Motor res{};
    res[0] = -b[6] * data[5] - b[5] * data[4] - b[4] * data[3];
    res[1] = b[0] * data[0] - b[6] * data[1] + b[5] * data[2] + b[2] * data[5] - b[3] * data[4] - b[7] * data[3];
    res[2] = b[6] * data[0] + b[0] * data[1] - b[4] * data[2] - b[1] * data[5] - b[7] * data[4] + b[3] * data[3];
    res[3] = -b[5] * data[0] + b[4] * data[1] + b[0] * data[2] - b[7] * data[5] + b[1] * data[4] - b[2] * data[3];
    res[4] = b[5] * data[5] - b[6] * data[4] + b[0] * data[3];
    res[5] = -b[4] * data[5] + b[0] * data[4] + b[6] * data[3];
    res[6] = b[0] * data[5] + b[4] * data[4] - b[5] * data[3];
    res[7] = b[4] * data[0] + b[5] * data[1] + b[6] * data[2] + b[3] * data[5] + b[2] * data[4] + b[1] * data[3];
    return res;
};
// OneBlade
[[nodiscard]] MultiVector OneBlade::operator* (const MultiVector& b) const
{
    MultiVector res{};
    res[0] = b[2] * data[1] + b[3] * data[2] + b[4] * data[3];
    res[1] = b[0] * data[0] - b[5] * data[1] - b[6] * data[2] - b[7] * data[3];
    res[2] = b[0] * data[1] - b[10] * data[2] + b[9] * data[3];
    res[3] = b[10] * data[1] + b[0] * data[2] - b[8] * data[3];
    res[4] = - b[9] * data[1] + b[8] * data[2] + b[0] * data[3];
    res[5] = b[2] * data[0] - b[1] * data[1] - b[13] * data[2] + b[12] * data[3];
    res[6] = b[3] * data[0] + b[13] * data[1] - b[1] * data[2] - b[11] * data[3];
    res[7] = b[4] * data[0] - b[12] * data[1] + b[11] * data[2] - b[1] * data[3];
    res[8] = b[14] * data[1] + b[4] * data[2] - b[3] * data[3];
    res[9] = - b[4] * data[1] + b[14] * data[2] + b[2] * data[3];
    res[10] = b[3] * data[1] - b[2] * data[2] + b[14] * data[3];
    res[11] = - b[8] * data[0] + b[15] * data[1] + b[7] * data[2] - b[6] * data[3];
    res[12] = - b[9] * data[0] - b[7] * data[1] + b[15] * data[2] + b[5] * data[3];
    res[13] = - b[10] * data[0] + b[6] * data[1] - b[5] * data[2] + b[15] * data[3];
    res[14] = b[8] * data[1] + b[9] * data[2] + b[10] * data[3];
    res[15] = b[14] * data[0] + b[11] * data[1] + b[12] * data[2] + b[13] * data[3];
    return res;
}
[[nodiscard]] Motor OneBlade::operator* (const ThreeBlade& b) const
{
    Motor res{};
    res[0] = 0;
    res[1] = -data[2] * b[2] + data[3] * b[1];
    res[2] = data[1] * b[2] - data[3] * b[0];
    res[3] = -data[1] * b[1] + data[2] * b[0];
    res[4] = data[1] * b[3];
    res[5] = data[2] * b[3];
    res[6] = data[3] * b[3];
    res[7] = -data[3] * b[2] - data[2] * b[1] - data[1] * b[0] - data[0] * b[3];
    return res;
}
[[nodiscard]] MultiVector OneBlade::operator* (const TwoBlade& b) const {
MultiVector res{};
res[1] = -b[0] * data[1] - b[1] * data[2] - b[2] * data[3];
res[2] = -b[5] * data[2] + b[4] * data[3];
res[3] = b[5] * data[1] - b[3] * data[3];
res[4] = -b[4] * data[1] + b[3] * data[2];
res[11] = -b[3] * data[0] + b[2] * data[2] - b[1] * data[3];
res[12] = -b[4] * data[0] - b[2] * data[1] + b[0] * data[3];
res[13] = -b[5] * data[0] + b[1] * data[1] - b[0] * data[2];
res[14] = b[3] * data[1] + b[4] * data[2] + b[5] * data[3];
return res;
};
[[nodiscard]] Motor OneBlade::operator* (const OneBlade& b) const
{
    Motor res{};
    res[0] = data[1] * b[1] + data[2] * b[2] + data[3] * b[3];
    res[1] = data[1] * b[0] - data[0] * b[1];
    res[2] = data[2] * b[0] - data[0] * b[2];
    res[3] = data[3] * b[0] - data[0] * b[3];
    res[4] = data[3] * b[2] - data[2] * b[3];
    res[5] = -data[3] * b[1] + data[1] * b[3];
    res[6] = data[2] * b[1] - data[1] * b[2];
    res[7] = 0;
    return res;
}
[[nodiscard]] MultiVector OneBlade::operator* (const Motor& b) const
{
    MultiVector res{};
    res[1] = b[0] * data[0] - b[1] * data[1] - b[2] * data[2] - b[3] * data[3];
    res[2] = b[0] * data[1] - b[6] * data[2] + b[5] * data[3];
    res[3] = b[6] * data[1] + b[0] * data[2] - b[4] * data[3];
    res[4] = -b[5] * data[1] + b[4] * data[2] + b[0] * data[3];
    res[11] = -b[4] * data[0] + b[7] * data[1] + b[3] * data[2] - b[2] * data[3];
    res[12] = -b[5] * data[0] - b[3] * data[1] + b[7] * data[2] + b[1] * data[3];
    res[13] = -b[6] * data[0] + b[2] * data[1] - b[1] * data[2] + b[7] * data[3];
    res[14] = b[4] * data[1] + b[5] * data[2] + b[6] * data[3];
    return res;
}
// Motor
[[nodiscard]] MultiVector Motor::operator* (const MultiVector& b) const
{
    MultiVector res{};
    res[0] = data[0] * b[0] - data[6] * b[10] - data[5] * b[9] - data[4] * b[8];
    res[1] = data[0] * b[1] - data[1] * b[2] - data[2] * b[3] - data[3] * b[4] + data[6] * b[13] + data[5] * b[12] + data[4] * b[11] + data[7] * b[14];
    res[2] = data[0] * b[2] - data[6] * b[3] + data[5] * b[4] - data[4] * b[14];
    res[3] = data[6] * b[2] + data[0] * b[3] - data[4] * b[4] - data[5] * b[14];
    res[4] = -data[5] * b[2] + data[4] * b[3] + data[0] * b[4] - data[6] * b[14];
    res[5] = data[1] * b[0] + data[0] * b[5] - data[6] * b[6] + data[5] * b[7] + data[2] * b[10] - data[3] * b[9] - data[7] * b[8] - data[4] * b[15];
    res[6] = data[2] * b[0] + data[6] * b[5] + data[0] * b[6] - data[4] * b[7] - data[1] * b[10] - data[7] * b[9] + data[3] * b[8] - data[5] * b[15];
    res[7] = data[3] * b[0] - data[5] * b[5] + data[4] * b[6] + data[0] * b[7] - data[7] * b[10] + data[1] * b[9] - data[2] * b[8] - data[6] * b[15];
    res[8] = data[4] * b[0] + data[5] * b[10] - data[6] * b[9] + data[0] * b[8];
    res[9] = data[5] * b[0] - data[4] * b[10] + data[0] * b[9] + data[6] * b[8];
    res[10] = data[6] * b[0] + data[0] * b[10] + data[4] * b[9] - data[5] * b[8];
    res[11] = -data[4] * b[1] + data[7] * b[2] + data[3] * b[3] - data[2] * b[4] + data[5] * b[13] - data[6] * b[12] + data[0] * b[11] + data[1] * b[14];
    res[12] = -data[5] * b[1] - data[3] * b[2] + data[7] * b[3] + data[1] * b[4] - data[4] * b[13] + data[0] * b[12] + data[6] * b[11] + data[2] * b[14];
    res[13] = -data[6] * b[1] + data[2] * b[2] - data[1] * b[3] + data[7] * b[4] + data[0] * b[13] + data[4] * b[12] - data[5] * b[11] + data[3] * b[14];
    res[14] = data[4] * b[2] + data[5] * b[3] + data[6] * b[4] + data[0] * b[14];
    res[15] = data[7] * b[0] + data[4] * b[5] + data[5] * b[6] + data[6] * b[7] + data[3] * b[10] + data[2] * b[9] + data[1] * b[8] + data[0] * b[15];
    return res;
}
[[nodiscard]] MultiVector Motor::operator* (const ThreeBlade& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = b[2] * data[6] + b[1] * data[5] + b[0] * data[4] - b[3] * data[7];
    res[2] = - data[4] * b[3];
    res[3] = - data[5] * b[3];
    res[4] = - data[6] * b[3];
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[11] = b[0] * data[0] - b[3] * data[1] + b[1] * data[6] - b[2] * data[5];
    res[12] = b[1] * data[0] - b[3] * data[2] - b[0] * data[6] + b[2] * data[4];
    res[13] = b[2] * data[0] - b[3] * data[3] + b[0] * data[5] - b[1] * data[4];
    res[14] = data[0] * b[3];
    res[15] = 0;
    return res;
}
[[nodiscard]] Motor Motor::operator* (const TwoBlade& b) const {
    Motor res{};
    res[0] = -b[5] * data[6] - b[4] * data[5] - b[3] * data[4];
    res[1] = b[0] * data[0] - b[5] * data[2] + b[4] * data[3] + b[1] * data[6] - b[2] * data[5] - b[3] * data[7];
    res[2] = b[1] * data[0] + b[5] * data[1] - b[3] * data[3] - b[0] * data[6] + b[2] * data[4] - b[4] * data[7];
    res[3] = b[2] * data[0] - b[4] * data[1] + b[3] * data[2] + b[0] * data[5] - b[1] * data[4] - b[5] * data[7];
    res[4] = b[3] * data[0] + b[4] * data[6] - b[5] * data[5];
    res[5] = b[4] * data[0] - b[3] * data[6] + b[5] * data[4];
    res[6] = b[5] * data[0] + b[3] * data[5] - b[4] * data[4];
    res[7] = b[3] * data[1] + b[4] * data[2] + b[5] * data[3] + b[2] * data[6] + b[1] * data[5] + b[0] * data[4];
    return res;
};
[[nodiscard]] MultiVector Motor::operator* (const OneBlade& b) const
{
    MultiVector res{};
    res[1] = data[0] * b[0] - data[1] * b[1] - data[2] * b[2] - data[3] * b[3];
    res[2] = data[0] * b[1] - data[6] * b[2] + data[5] * b[3];
    res[3] = data[6] * b[1] + data[0] * b[2] - data[4] * b[3];
    res[4] = -data[5] * b[1] + data[4] * b[2] + data[0] * b[3];
    res[11] = -data[4] * b[0] + data[7] * b[1] + data[3] * b[2] - data[2] * b[3];
    res[12] = -data[5] * b[0] - data[3] * b[1] + data[7] * b[2] + data[1] * b[3];
    res[13] = -data[6] * b[0] + data[2] * b[1] - data[1] * b[2] + data[7] * b[3];
    res[14] = data[4] * b[1] + data[5] * b[2] + data[6] * b[3];
    return res;
}
[[nodiscard]] Motor Motor::operator* (const Motor& b) const {
    Motor res{};
    res[0] = b[0] * data[0] - b[6] * data[6] - b[5] * data[5] - b[4] * data[4];
    res[1] = b[1] * data[0] + b[0] * data[1] - b[6] * data[2] + b[5] * data[3] + b[2] * data[6] - b[3] * data[5] - b[7] * data[4] - b[4] * data[7];
    res[2] = b[2] * data[0] + b[6] * data[1] + b[0] * data[2] - b[4] * data[3] - b[1] * data[6] - b[7] * data[5] + b[3] * data[4] - b[5] * data[7];
    res[3] = b[3] * data[0] - b[5] * data[1] + b[4] * data[2] + b[0] * data[3] - b[7] * data[6] + b[1] * data[5] - b[2] * data[4] - b[6] * data[7];
    res[4] = b[4] * data[0] + b[5] * data[6] - b[6] * data[5] + b[0] * data[4];
    res[5] = b[5] * data[0] - b[4] * data[6] + b[0] * data[5] + b[6] * data[4];
    res[6] = b[6] * data[0] + b[0] * data[6] + b[4] * data[5] - b[5] * data[4];
    res[7] = b[7] * data[0] + b[4] * data[1] + b[5] * data[2] + b[6] * data[3] + b[3] * data[6] + b[2] * data[5] + b[1] * data[4] + b[0] * data[7];
    return res;
};

// Inner

// MultiVector
[[nodiscard]] MultiVector MultiVector::operator| (const MultiVector& b) const
{
    MultiVector res{};
    res[0] = b[0] * data[0] + b[2] * data[2] + b[3] * data[3] + b[4] * data[4] - b[10] * data[10] - b[9] * data[9] - b[8] * data[8] - b[14] * data[14];
    res[1] = b[1] * data[0] + b[0] * data[1] - b[5] * data[2] - b[6] * data[3] - b[7] * data[4] + b[2] * data[5] + b[3] * data[6] + b[4] * data[7] + b[13] * data[10] + b[12] * data[9] + b[11] * data[8] + b[10] * data[13] + b[9] * data[12] + b[8] * data[11] + b[15] * data[14] - b[14] * data[15];
    res[2] = b[2] * data[0] + b[0] * data[2] - b[10] * data[3] + b[9] * data[4] + b[3] * data[10] - b[4] * data[9] - b[14] * data[8] - b[8] * data[14];
    res[3] = b[3] * data[0] + b[10] * data[2] + b[0] * data[3] - b[8] * data[4] - b[2] * data[10] - b[14] * data[9] + b[4] * data[8] - b[9] * data[14];
    res[4] = b[4] * data[0] - b[9] * data[2] + b[8] * data[3] + b[0] * data[4] - b[14] * data[10] + b[2] * data[9] - b[3] * data[8] - b[10] * data[14];
    res[5] = b[5] * data[0] - b[13] * data[3] + b[12] * data[4] + b[0] * data[5] - b[15] * data[8] - b[3] * data[13] + b[4] * data[12] - b[8] * data[15];
    res[6] = b[6] * data[0] + b[13] * data[2] - b[11] * data[4] + b[0] * data[6] - b[15] * data[9] + b[2] * data[13] - b[4] * data[11] - b[9] * data[15];
    res[7] = b[7] * data[0] - b[12] * data[2] + b[11] * data[3] + b[0] * data[7] - b[15] * data[10] - b[2] * data[12] + b[3] * data[11] - b[10] * data[15];
    res[10] = b[10] * data[0] + b[14] * data[4] + b[0] * data[10] + b[4] * data[14];
    res[9] = b[9] * data[0] + b[14] * data[3] + b[0] * data[9] + b[3] * data[14];
    res[8] = b[8] * data[0] + b[14] * data[2] + b[0] * data[8] + b[2] * data[14];
    res[13] = b[13] * data[0] + b[15] * data[4] + b[0] * data[13] - b[4] * data[15];
    res[12] = b[12] * data[0] + b[15] * data[3] + b[0] * data[12] - b[3] * data[15];
    res[11] = b[11] * data[0] + b[15] * data[2] + b[0] * data[11] - b[2] * data[15];
    res[14] = b[14] * data[0] + b[0] * data[14];
    res[15] = b[15] * data[0] + b[0] * data[15];
    return res;
};
[[nodiscard]] MultiVector MultiVector::operator| (const ThreeBlade& b) const
{
    MultiVector res{};
    res[0] = - b[3] * data[14];
    res[1] = b[2] * data[10] + b[1] * data[9] + b[0] * data[8] - b[3] * data[15];
    res[2] = - b[3] * data[8];
    res[3] = - b[3] * data[9];
    res[4] = - b[3] * data[10];
    res[5] = - b[2] * data[3] + b[1] * data[4];
    res[6] = b[2] * data[2] - b[0] * data[4];
    res[7] = - b[1] * data[2] + b[0] * data[3];
    res[10] = b[3] * data[4];
    res[9] = b[3] * data[3];
    res[8] = b[3] * data[2];
    res[13] = b[2] * data[0];
    res[12] = b[1] * data[0];
    res[11] = b[0] * data[0];
    res[14] = b[3] * data[0];
    res[15] = 0;
    return res;
};
[[nodiscard]] MultiVector MultiVector::operator| (const TwoBlade& b) const
{
    MultiVector res{};
    res[0] = - b[5] * data[10] - b[4] * data[9] - b[3] * data[8];
    res[1] = - b[0] * data[2] - b[1] * data[3] - b[2] * data[4] + b[5] * data[13] + b[4] * data[12] + b[3] * data[11];
    res[2] = - b[5] * data[3] + b[4] * data[4] - b[3] * data[14];
    res[3] = b[5] * data[2] - b[3] * data[4] - b[4] * data[14];
    res[4] = -b[4] * data[2] + b[3] * data[3] - b[5] * data[14];
    res[5] = b[0] * data[0] - b[3] * data[15];
    res[6] = b[1] * data[0] - b[4] * data[15];
    res[7] = b[2] * data[0] - b[5] * data[15];
    res[10] = b[5] * data[0];
    res[9] = b[4] * data[0];
    res[8] = b[3] * data[0];
    res[13] = 0;
    res[12] = 0;
    res[11] = 0;
    res[14] = 0;
    res[15] = 0;
    return res;
};
[[nodiscard]] MultiVector MultiVector::operator| (const OneBlade& b) const
{
    MultiVector res{};
    res[0] = b[1] * data[2] + b[2] * data[3] + b[3] * data[4];
    res[1] = b[0] * data[0] + b[1] * data[5] + b[2] * data[6] + b[3] * data[7];
    res[2] = b[1] * data[0] + b[2] * data[10] - b[3] * data[9];
    res[3] = b[2] * data[0] - b[1] * data[10] + b[3] * data[8];
    res[4] = b[3] * data[0] + b[1] * data[9] - b[2] * data[8];
    res[5] = - b[2] * data[13] + b[3] * data[12];
    res[6] = b[1] * data[13] - b[3] * data[11];
    res[7] = - b[1] * data[12] + b[2] * data[11];
    res[10] = b[3] * data[14];
    res[9] = b[2] * data[14];
    res[8] = b[1] * data[14];
    res[13] = - b[3] * data[15];
    res[12] = - b[2] * data[15];
    res[11] = - b[1] * data[15];
    res[14] = 0;
    res[15] = 0;
    return res;
};
[[nodiscard]] MultiVector MultiVector::operator| (const Motor& b) const
{
    MultiVector res{};
    res[0] = b[0] * data[0] - b[6] * data[10] - b[5] * data[9] - b[4] * data[8];
    res[1] = b[0] * data[1] - b[1] * data[2] - b[2] * data[3] - b[3] * data[4] + b[6] * data[13] + b[5] * data[12] + b[4] * data[11] + b[7] * data[14];
    res[2] = b[0] * data[2] - b[6] * data[3] + b[5] * data[4] - b[4] * data[14];
    res[3] = b[6] * data[2] + b[0] * data[3] - b[4] * data[4] - b[5] * data[14];
    res[4] = - b[5] * data[2] + b[4] * data[3] + b[0] * data[4] - b[6] * data[14];
    res[5] = b[1] * data[0] + b[0] * data[5] - b[7] * data[8] - b[4] * data[15];
    res[6] = b[2] * data[0] + b[0] * data[6] - b[7] * data[9] - b[5] * data[15];
    res[7] = b[3] * data[0] + b[0] * data[7] - b[7] * data[10] - b[6] * data[15];
    res[10] = b[6] * data[0] + b[0] * data[10];
    res[9] = b[5] * data[0] + b[0] * data[9];
    res[8] = b[4] * data[0] + b[0] * data[8];
    res[13] = b[7] * data[4] + b[0] * data[13];
    res[12] = b[7] * data[3] + b[0] * data[12];
    res[11] = b[7] * data[2] + b[0] * data[11];
    res[14] = b[0] * data[14];
    res[15] = b[0] * data[15];
    return res;
};
// ThreeBlade
[[nodiscard]] MultiVector ThreeBlade::operator| (const MultiVector& b) const
{
    MultiVector res{};
    res[0] = -data[3] * b[14];
    res[1] = data[2] * b[10] + data[1] * b[9] + data[0] * b[8] - data[3] * b[15];
    res[2] = -data[3] * b[8];
    res[3] = -data[3] * b[9];
    res[4] = -data[3] * b[10];
    res[5] = -data[2] * b[3] + data[1] * b[4];
    res[6] = data[2] * b[2] - data[0] * b[4];
    res[7] = -data[1] * b[2] + data[0] * b[3];
    res[10] = data[3] * b[4];
    res[9] = data[3] * b[3];
    res[8] = data[3] * b[2];
    res[13] = data[2] * b[0];
    res[12] = data[1] * b[0];
    res[11] = data[0] * b[0];
    res[14] = data[3] * b[0];
    res[15] = 0;
    return res;
};
[[nodiscard]] float ThreeBlade::operator| (const ThreeBlade & b) const
{
    return -data[3] * b[3];
};
[[nodiscard]] OneBlade ThreeBlade::operator| (const TwoBlade& b) const
{
    OneBlade res{};
    res[0] = data[2] * b[5] + data[1] * b[4] + data[0] * b[3];
    res[1] = -data[3] * b[3];
    res[2] = -data[3] * b[4];
    res[3] = -data[3] * b[5];
    return res;
};
[[nodiscard]] TwoBlade ThreeBlade::operator| (const OneBlade& b) const
{
    TwoBlade res{};
    res[0] = -data[2] * b[2] + data[1] * b[3];
    res[1] = data[2] * b[1] - data[0] * b[3];
    res[2] = -data[1] * b[1] + data[0] * b[2];
    res[3] = data[3] * b[1];
    res[4] = data[3] * b[2];
    res[5] = data[3] * b[3];
    return res;
};
[[nodiscard]] OneBlade ThreeBlade::operator| (const Motor& b) const
{
    OneBlade res{};
    res[0] = data[2] * b[6] + data[1] * b[5] + data[0] * b[4] - data[3] * b[7];
    res[1] = -data[3] * b[4];
    res[2] = -data[3] * b[5];
    res[3] = -data[3] * b[6];
    return res;
};
// TwoBlade
[[nodiscard]] MultiVector TwoBlade::operator| (const MultiVector& b) const
{
    MultiVector res{};
    res[0] = -data[5] * b[10] - data[4] * b[9] - data[3] * b[8];
    res[1] = -data[0] * b[2] - data[1] * b[3] - data[2] * b[4] + data[5] * b[13] + data[4] * b[12] + data[3] * b[11];
    res[2] = -data[5] * b[3] + data[4] * b[4] - data[3] * b[14];
    res[3] = data[5] * b[2] - data[3] * b[4] - data[4] * b[14];
    res[4] = -data[4] * b[2] + data[3] * b[3] - data[5] * b[14];
    res[5] = data[0] * b[0] - data[3] * b[15];
    res[6] = data[1] * b[0] - data[4] * b[15];
    res[7] = data[2] * b[0] - data[5] * b[15];
    res[10] = data[5] * b[0];
    res[9] = data[4] * b[0];
    res[8] = data[3] * b[0];
    res[13] = 0;
    res[12] = 0;
    res[11] = 0;
    res[14] = 0;
    res[15] = 0;
    return res;
};
[[nodiscard]] OneBlade TwoBlade::operator| (const ThreeBlade& b) const
{
    OneBlade res{};
    res[0] = data[5] * b[2] + data[4] * b[1] + data[3] * b[0];
    res[1] = - data[3] * b[3];
    res[2] = - data[4] * b[3];
    res[3] = - data[5] * b[3];
    return res;
};
[[nodiscard]] float TwoBlade::operator| (const TwoBlade& b) const
{
    return -b[5] * data[5] - b[4] * data[4] - b[3] * data[3];
};
[[nodiscard]] OneBlade TwoBlade::operator| (const OneBlade& b) const
{
    OneBlade res{};
    res[0] = -data[0] * b[1] - data[1] * b[2] - data[2] * b[3];
    res[1] = -data[5] * b[2] + data[4] * b[3];
    res[2] = data[5] * b[1] - data[3] * b[3];
    res[3] = -data[4] * b[1] + data[3] * b[2];
    return res;
};
[[nodiscard]] Motor TwoBlade::operator| (const Motor& b) const
{
    Motor res{};
    res[0] = -data[5] * b[6] - data[4] * b[5] - data[3] * b[4];
    res[1] = data[0] * b[0] - data[3] * b[7];
    res[2] = data[1] * b[0] - data[4] * b[7];
    res[3] = data[2] * b[0] - data[5] * b[7];
    res[6] = data[5] * b[0];
    res[5] = data[4] * b[0];
    res[4] = data[3] * b[0];
    return res;
};
// Oneblade
[[nodiscard]] MultiVector OneBlade::operator| (const MultiVector& b) const
{
    MultiVector res{};
    res[0] = data[1] * b[2] + data[2] * b[3] + data[3] * b[4];
    res[1] = data[0] * b[0] + data[1] * b[5] + data[2] * b[6] + data[3] * b[7];
    res[2] = data[1] * b[0] + data[2] * b[10] - data[3] * b[9];
    res[3] = data[2] * b[0] - data[1] * b[10] + data[3] * b[8];
    res[4] = data[3] * b[0] + data[1] * b[9] - data[2] * b[8];
    res[5] = -data[2] * b[13] + data[3] * b[12];
    res[6] = data[1] * b[13] - data[3] * b[11];
    res[7] = -data[1] * b[12] + data[2] * b[11];
    res[10] = data[3] * b[14];
    res[9] = data[2] * b[14];
    res[8] = data[1] * b[14];
    res[13] = -data[3] * b[15];
    res[12] = -data[2] * b[15];
    res[11] = -data[1] * b[15];
    res[14] = 0;
    res[15] = 0;
    return res;
};
[[nodiscard]] TwoBlade OneBlade::operator| (const ThreeBlade& b) const
{
    TwoBlade res{};
    res[0] = -data[2] * b[2] + data[3] * b[1];
    res[1] = data[1] * b[2] - data[3] * b[0];
    res[2] = -data[1] * b[1] + data[2] * b[0];
    res[5] = data[3] * b[3];
    res[4] = data[2] * b[3];
    res[3] = data[1] * b[3];
    return res;
};
[[nodiscard]] OneBlade OneBlade::operator| (const TwoBlade& b) const
{
    OneBlade res{};
    res[0] = -b[0] * data[1] - b[1] * data[2] - b[2] * data[3];
    res[1] = -b[5] * data[2] + b[4] * data[3];
    res[2] = b[5] * data[1] - b[3] * data[3];
    res[3] = -b[4] * data[1] + b[3] * data[2];
    return res;
}
[[nodiscard]] float OneBlade::operator| (const OneBlade& b) const
{
    return data[1] * b[1] + data[2] * b[2] + data[3] * b[3];
};
[[nodiscard]] MultiVector OneBlade::operator| (const Motor& b) const
{
    MultiVector res{};
    res[1] = data[0] * b[0] + data[1] * b[1] + data[2] * b[2] + data[3] * b[3];
    res[2] = data[1] * b[0] + data[2] * b[6] - data[3] * b[5];
    res[3] = data[2] * b[0] - data[1] * b[6] + data[3] * b[4];
    res[4] = data[3] * b[0] + data[1] * b[5] - data[2] * b[4];
    res[13] = -data[3] * b[7];
    res[12] = -data[2] * b[7];
    res[11] = -data[1] * b[7];
    return res;
};
// Motor
[[nodiscard]] MultiVector Motor::operator| (const MultiVector& b) const
{
    MultiVector res{};
    res[0] = data[0] * b[0] - data[6] * b[10] - data[5] * b[9] - data[4] * b[8];
    res[1] = data[0] * b[1] - data[1] * b[2] - data[2] * b[3] - data[3] * b[4] + data[6] * b[13] + data[5] * b[12] + data[4] * b[11] + data[7] * b[14];
    res[2] = data[0] * b[2] - data[6] * b[3] + data[5] * b[4] - data[4] * b[14];
    res[3] = data[6] * b[2] + data[0] * b[3] - data[4] * b[4] - data[5] * b[14];
    res[4] = -data[5] * b[2] + data[4] * b[3] + data[0] * b[4] - data[6] * b[14];
    res[5] = data[1] * b[0] + data[0] * b[5] - data[7] * b[8] - data[4] * b[15];
    res[6] = data[2] * b[0] + data[0] * b[6] - data[7] * b[9] - data[5] * b[15];
    res[7] = data[3] * b[0] + data[0] * b[7] - data[7] * b[10] - data[6] * b[15];
    res[10] = data[6] * b[0] + data[0] * b[10];
    res[9] = data[5] * b[0] + data[0] * b[9];
    res[8] = data[4] * b[0] + data[0] * b[8];
    res[13] = data[7] * b[4] + data[0] * b[13];
    res[12] = data[7] * b[3] + data[0] * b[12];
    res[11] = data[7] * b[2] + data[0] * b[11];
    res[14] = data[0] * b[14];
    res[15] = data[0] * b[15];
    return res;
};
[[nodiscard]] MultiVector Motor::operator| (const ThreeBlade& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = data[6] * b[2] + data[5] * b[1] + data[4] * b[0] + data[7] * b[3];
    res[2] = - data[4] * b[3];
    res[3] = - data[5] * b[3];
    res[4] = - data[6] * b[3];
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[13] = data[0] * b[2];
    res[12] = data[0] * b[1];
    res[11] = data[0] * b[0];
    res[14] = data[0] * b[3];
    res[15] = 0;
    return res;
};
[[nodiscard]] Motor Motor::operator| (const TwoBlade& b) const
{
    Motor res{};
    res[0] = - data[6] * b[5] - data[5] * b[4] - data[4] * b[3];
    res[1] = data[0] * b[0] - data[7] * b[3];
    res[2] = data[0] * b[1] - data[7] * b[4];
    res[3] = data[0] * b[2] - data[7] * b[5];
    res[6] = data[0] * b[5];
    res[5] = data[0] * b[4];
    res[4] = data[0] * b[3];
    res[7] = 0;
    return res;
};
[[nodiscard]] MultiVector Motor::operator| (const OneBlade& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = data[0] * b[0] - data[1] * b[1] - data[2] * b[2] - data[3] * b[3];
    res[2] = data[0] * b[1] - data[6] * b[2] + data[5] * b[3];
    res[3] = data[6] * b[1] + data[0] * b[2] - data[4] * b[3];
    res[4] = -data[5] * b[1] + data[4] * b[2] + data[0] * b[3];
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[13] = data[7] * b[3];
    res[12] = data[7] * b[2];
    res[11] = data[7] * b[1];
    res[14] = 0;
    res[15] = 0;
    return res;
};
[[nodiscard]] Motor Motor::operator| (const Motor& b) const
{
    Motor res{};
    res[0] = data[0] * b[0] - data[6] * b[6] - data[5] * b[5] - data[4] * b[4];
    res[1] = data[1] * b[0] + data[0] * b[1] - data[7] * b[4] - data[4] * b[7];
    res[2] = data[2] * b[0] + data[0] * b[2] - data[7] * b[5] - data[5] * b[7];
    res[3] = data[3] * b[0] + data[0] * b[3] - data[7] * b[6] - data[6] * b[7];
    res[6] = data[6] * b[0] + data[0] * b[6];
    res[5] = data[5] * b[0] + data[0] * b[5];
    res[4] = data[4] * b[0] + data[0] * b[4];
    res[7] = data[0] * b[7];
    return res;
};

// Outer Product

// MultiVector
[[nodiscard]] MultiVector MultiVector::operator^(const MultiVector& b) const
{
    MultiVector res{};
    res[0] = b[0] * data[0];
    res[1] = b[1] * data[0] + b[0] * data[1];
    res[2] = b[2] * data[0] + b[0] * data[2];
    res[3] = b[3] * data[0] + b[0] * data[3];
    res[4] = b[4] * data[0] + b[0] * data[4];
    res[5] = b[5] * data[0] + b[2] * data[1] - b[1] * data[2] + b[0] * data[5];
    res[6] = b[6] * data[0] + b[3] * data[1] - b[1] * data[3] + b[0] * data[6];
    res[7] = b[7] * data[0] + b[4] * data[1] - b[1] * data[4] + b[0] * data[7];
    res[10] = b[10] * data[0] + b[3] * data[2] - b[2] * data[3] + b[0] * data[10];
    res[9] = b[9] * data[0] - b[4] * data[2] + b[2] * data[4] + b[0] * data[9];
    res[8] = b[8] * data[0] + b[4] * data[3] - b[3] * data[4] + b[0] * data[8];
    res[13] = b[13] * data[0] - b[10] * data[1] + b[6] * data[2] - b[5] * data[3] - b[3] * data[5] + b[2] * data[6] - b[1] * data[10] + b[0] * data[13];
    res[12] = b[12] * data[0] - b[9] * data[1] - b[7] * data[2] + b[5] * data[4] + b[4] * data[5] - b[2] * data[7] - b[1] * data[9] + b[0] * data[12];
    res[11] = b[11] * data[0] - b[8] * data[1] + b[7] * data[3] - b[6] * data[4] - b[4] * data[6] + b[3] * data[7] - b[1] * data[8] + b[0] * data[11];
    res[14] = b[14] * data[0] + b[8] * data[2] + b[9] * data[3] + b[10] * data[4] + b[4] * data[10] + b[3] * data[9] + b[2] * data[8] + b[0] * data[14];
    res[15] = b[15] * data[0] + b[14] * data[1] + b[11] * data[2] + b[12] * data[3] + b[13] * data[4] + b[8] * data[5] + b[9] * data[6] + b[10] * data[7] + b[7] * data[10] + b[6] * data[9] + b[5] * data[8] - b[4] * data[13] - b[3] * data[12] - b[2] * data[11] - b[1] * data[14] + b[0] * data[15];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator^(const ThreeBlade& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = 0;
    res[2] = 0;
    res[3] = 0;
    res[4] = 0;
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[10] = 0;
    res[9] = 0;
    res[8] = 0;
    res[13] = b[2] * data[0];
    res[12] = b[1] * data[0];
    res[11] = b[0] * data[0];
    res[14] = b[3] * data[0];
    res[15] = b[3] * data[1] + b[0] * data[2] + b[1] * data[3] + b[2] * data[4];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator^(const TwoBlade& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = 0;
    res[2] = 0;
    res[3] = 0;
    res[4] = 0;
    res[5] = b[0] * data[0];
    res[6] = b[1] * data[0];
    res[7] = b[2] * data[0];
    res[10] = b[5] * data[0];
    res[9] = b[4] * data[0];
    res[8] = b[3] * data[0];
    res[13] = - b[5] * data[1] + b[1] * data[2] - b[0] * data[3];
    res[12] = - b[4] * data[1] - b[2] * data[2] + b[0] * data[4];
    res[11] = - b[3] * data[1] + b[2] * data[3] - b[1] * data[4];
    res[14] = b[3] * data[2] + b[4] * data[3] + b[5] * data[4];
    res[15] = b[3] * data[5] + b[4] * data[6] + b[5] * data[7] + b[2] * data[10] + b[1] * data[9] + b[0] * data[8];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator^(const OneBlade& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = b[0] * data[0];
    res[2] = b[1] * data[0];
    res[3] = b[2] * data[0];
    res[4] = b[3] * data[0];
    res[5] = b[1] * data[1] - b[0] * data[2];
    res[6] = b[2] * data[1] - b[0] * data[3];
    res[7] = b[3] * data[1] - b[0] * data[4];
    res[10] = b[2] * data[2] - b[1] * data[3];
    res[9] = - b[3] * data[2] + b[1] * data[4];
    res[8] = b[3] * data[3] - b[2] * data[4];
    res[13] = - b[2] * data[5] + b[1] * data[6] - b[0] * data[10];
    res[12] = b[3] * data[5] - b[1] * data[7] - b[0] * data[9];
    res[11] = - b[3] * data[6] + b[2] * data[7] - b[0] * data[8];
    res[14] = b[3] * data[10] + b[2] * data[9] + b[1] * data[8];
    res[15] = - b[3] * data[13] - b[2] * data[12] - b[1] * data[11] - b[0] * data[14];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator^(const Motor& b) const
{
    MultiVector res{};
    res[0] = b[0] * data[0];
    res[1] = b[0] * data[1];
    res[2] = b[0] * data[2];
    res[3] = b[0] * data[3];
    res[4] = b[0] * data[4];
    res[5] = b[1] * data[0] + b[0] * data[5];
    res[6] = b[2] * data[0] + b[0] * data[6];
    res[7] = b[3] * data[0] + b[0] * data[7];
    res[10] = b[6] * data[0] + b[0] * data[10];
    res[9] = b[5] * data[0] + b[0] * data[9];
    res[8] = b[4] * data[0] + b[0] * data[8];
    res[13] = - b[6] * data[1] + b[0] * data[13];
    res[12] = - b[5] * data[1] + b[0] * data[12];
    res[11] = - b[4] * data[1] + b[0] * data[11];
    res[14] = b[4] * data[2] + b[5] * data[3] + b[6] * data[4] + b[0] * data[14];
    res[15] = b[7] * data[0] + b[4] * data[5] + b[5] * data[6] + b[6] * data[7] + b[3] * data[10] + b[2] * data[9] + b[1] * data[8] + b[0] * data[15];
    return res;
}
// ThreeBlade
[[nodiscard]] MultiVector ThreeBlade::operator^(const MultiVector& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = 0;
    res[2] = 0;
    res[3] = 0;
    res[4] = 0;
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[10] = 0;
    res[9] = 0;
    res[8] = 0;
    res[13] = data[2] * b[0];
    res[12] = data[1] * b[0];
    res[11] = data[0] * b[0];
    res[14] = data[3] * b[0];
    res[15] = data[3] * b[1] + data[0] * b[2] + data[1] * b[3] + data[2] * b[4];
    return res;
}
[[nodiscard]] GANull ThreeBlade::operator^(const ThreeBlade& b) const
{
    return GANull{};
}
[[nodiscard]] GANull ThreeBlade::operator^(const TwoBlade& b) const
{
    return GANull{};
}
[[nodiscard]] float ThreeBlade::operator^(const OneBlade& b) const
{
    return data[3] * b[0] + data[0] * b[1] + data[1] * b[2] + data[2] * b[3];
}
[[nodiscard]] ThreeBlade ThreeBlade::operator^(const Motor& b) const
{
    ThreeBlade res{};
    res[2] = data[2] * b[0];
    res[1] = data[1] * b[0];
    res[0] = data[0] * b[0];
    res[3] = data[3] * b[0];
    return res;
}
// TwoBlade
[[nodiscard]] MultiVector TwoBlade::operator^(const MultiVector& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = 0;
    res[2] = 0;
    res[3] = 0;
    res[4] = 0;
    res[5] = data[0] * b[0];
    res[6] = data[1] * b[0];
    res[7] = data[2] * b[0];
    res[10] = data[5] * b[0];
    res[9] = data[4] * b[0];
    res[8] = data[3] * b[0];
    res[13] = -data[5] * b[1] + data[1] * b[2] - data[0] * b[3];
    res[12] = -data[4] * b[1] - data[2] * b[2] + data[0] * b[4];
    res[11] = -data[3] * b[1] + data[2] * b[3] - data[1] * b[4];
    res[14] = data[3] * b[2] + data[4] * b[3] + data[5] * b[4];
    res[15] = data[3] * b[5] + data[4] * b[6] + data[5] * b[7] + data[2] * b[10] + data[1] * b[9] + data[0] * b[8];
    return res;
}
[[nodiscard]] GANull TwoBlade::operator^(const ThreeBlade& b) const
{
    return GANull{};
}
[[nodiscard]] MultiVector TwoBlade::operator ^ (const TwoBlade& b) const
{
    MultiVector res{};
    res[15] = data[0] + b[4] * data[1] + b[5] * data[2] + b[2] * data[5] + b[1] * data[4] + b[0] * data[3];
    return res;
}
[[nodiscard]] ThreeBlade TwoBlade::operator^(const OneBlade& b) const
{
    ThreeBlade res{};
    res[2] = -data[5] * b[0] + data[1] * b[1] - data[0] * b[2];
    res[1] = -data[4] * b[0] - data[2] * b[1] + data[0] * b[3];
    res[0] = -data[3] * b[0] + data[2] * b[2] - data[1] * b[3];
    res[3] = data[3] * b[1] + data[4] * b[2] + data[5] * b[3];
    return res;
}
[[nodiscard]] Motor TwoBlade::operator^(const Motor& b) const
{
    Motor res{};
    res[0] = 0;
    res[1] = data[0] * b[0];
    res[2] = data[1] * b[0];
    res[3] = data[2] * b[0];
    res[6] = data[5] * b[0];
    res[5] = data[4] * b[0];
    res[4] = data[3] * b[0];
    res[7] = data[3] * b[1] + data[4] * b[2] + data[5] * b[3] + data[2] * b[6] + data[1] * b[5] + data[0] * b[4];
    return res;
}
// Oneblade
[[nodiscard]] MultiVector OneBlade::operator^(const MultiVector& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = data[0] * b[0];
    res[2] = data[1] * b[0];
    res[3] = data[2] * b[0];
    res[4] = data[3] * b[0];
    res[5] = data[1] * b[1] - data[0] * b[2];
    res[6] = data[2] * b[1] - data[0] * b[3];
    res[7] = data[3] * b[1] - data[0] * b[4];
    res[10] = data[2] * b[2] - data[1] * b[3];
    res[9] = -data[3] * b[2] + data[1] * b[4];
    res[8] = data[3] * b[3] - data[2] * b[4];
    res[13] = -data[2] * b[5] + data[1] * b[6] - data[0] * b[10];
    res[12] = data[3] * b[5] - data[1] * b[7] - data[0] * b[9];
    res[11] = -data[3] * b[6] + data[2] * b[7] - data[0] * b[8];
    res[14] = data[3] * b[10] + data[2] * b[9] + data[1] * b[8];
    res[15] = -data[3] * b[13] - data[2] * b[12] - data[1] * b[11] - data[0] * b[14];
    return res;
}
[[nodiscard]] MultiVector OneBlade::operator^(const ThreeBlade& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = 0;
    res[2] = 0;
    res[3] = 0;
    res[4] = 0;
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[10] = 0;
    res[9] = 0;
    res[8] = 0;
    res[13] = 0;
    res[12] = 0;
    res[11] = 0;
    res[14] = 0;
    res[15] = -data[3] * b[2] - data[2] * b[1] - data[1] * b[0] - data[0] * b[3];
    return res;
}
[[nodiscard]] ThreeBlade OneBlade::operator^ (const TwoBlade& b) const
{
    ThreeBlade res{};
    res[2] = -b[5] * data[0] + b[1] * data[1] - b[0] * data[2];
    res[1] = -b[4] * data[0] - b[2] * data[1] + b[0] * data[3];
    res[0] = -b[3] * data[0] + b[2] * data[2] - b[1] * data[3];
    res[3] = b[3] * data[1] + b[4] * data[2] + b[5] * data[3];
    return res;
}
[[nodiscard]] TwoBlade OneBlade::operator^(const OneBlade& b) const
{
    TwoBlade res{};
    res[0] = data[1] * b[0] - data[0] * b[1];
    res[1] = data[2] * b[0] - data[0] * b[2];
    res[2] = data[3] * b[0] - data[0] * b[3];
    res[5] = data[2] * b[1] - data[1] * b[2];
    res[4] = -data[3] * b[1] + data[1] * b[3];
    res[3] = data[3] * b[2] - data[2] * b[3];
    return res;
}
[[nodiscard]] MultiVector OneBlade::operator^(const Motor& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = data[0] * b[0];
    res[2] = data[1] * b[0];
    res[3] = data[2] * b[0];
    res[4] = data[3] * b[0];
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[10] = 0;
    res[9] = 0;
    res[8] = 0;
    res[13] = -data[2] * b[1] + data[1] * b[2] - data[0] * b[6];
    res[12] = data[3] * b[1] - data[1] * b[3] - data[0] * b[5];
    res[11] = -data[3] * b[2] + data[2] * b[3] - data[0] * b[3];
    res[14] = data[3] * b[6] + data[2] * b[5] + data[1] * b[3];
    res[15] = 0;
    return res;
}
// Motor
[[nodiscard]] MultiVector Motor::operator^(const MultiVector& b) const
{
    MultiVector res{};
    res[0] = data[0] * b[0];
    res[1] = data[0] * b[1];
    res[2] = data[0] * b[2];
    res[3] = data[0] * b[3];
    res[4] = data[0] * b[4];
    res[5] = data[1] * b[0] + data[0] * b[5];
    res[6] = data[2] * b[0] + data[0] * b[6];
    res[7] = data[3] * b[0] + data[0] * b[7];
    res[10] = data[6] * b[0] + data[0] * b[10];
    res[9] = data[5] * b[0] + data[0] * b[9];
    res[8] = data[4] * b[0] + data[0] * b[8];
    res[13] = -data[6] * b[1] + data[0] * b[13];
    res[12] = -data[5] * b[1] + data[0] * b[12];
    res[11] = -data[4] * b[1] + data[0] * b[11];
    res[14] = data[4] * b[2] + data[5] * b[3] + data[6] * b[4] + data[0] * b[14];
    res[15] = data[7] * b[0] + data[4] * b[5] + data[5] * b[6] + data[6] * b[7] + data[3] * b[10] + data[2] * b[9] + data[1] * b[8] + data[0] * b[15];
    return res;
}
[[nodiscard]] ThreeBlade Motor::operator^(const ThreeBlade& b) const
{
    ThreeBlade res{};
    res[2] = data[0] * b[2];
    res[1] = data[0] * b[1];
    res[0] = data[0] * b[0];
    res[3] = data[0] * b[3];
    return res;
}
[[nodiscard]] Motor Motor::operator^(const TwoBlade& b) const
{
    Motor res{};
    res[0] = 0;
    res[1] = data[0] * b[0];
    res[2] = data[0] * b[1];
    res[3] = data[0] * b[2];
    res[6] = data[0] * b[5];
    res[5] = data[0] * b[4];
    res[4] = data[0] * b[3];
    res[7] = data[4] * b[0] + data[5] * b[1] + data[6] * b[2] + data[3] * b[5] + data[2] * b[4] + data[1] * b[3];
    return res;
}
[[nodiscard]] MultiVector Motor::operator^(const OneBlade& b) const
{
    MultiVector res{};
    res[0] = 0;
    res[1] = data[0] * b[0];
    res[2] = data[0] * b[1];
    res[3] = data[0] * b[2];
    res[4] = data[0] * b[3];
    res[5] = 0;
    res[6] = 0;
    res[7] = 0;
    res[10] = 0;
    res[9] = 0;
    res[8] = 0;
    res[13] = -data[6] * b[0];
    res[12] = -data[5] * b[0];
    res[11] = -data[4] * b[0];
    res[14] = data[4] * b[1] + data[5] * b[2] + data[6] * b[3];
    res[15] = 0;
    return res;
}
[[nodiscard]] MultiVector Motor::operator^(const Motor& b) const
{
    MultiVector res{};
    res[0] = data[0] * b[0];
    res[1] = 0;
    res[2] = 0;
    res[3] = 0;
    res[4] = 0;
    res[5] = data[1] * b[0] + data[0] * b[1];
    res[6] = data[2] * b[0] + data[0] * b[2];
    res[7] = data[3] * b[0] + data[0] * b[3];
    res[10] = data[6] * b[0] + data[0] * b[6];
    res[9] = data[5] * b[0] + data[0] * b[5];
    res[8] = data[4] * b[0] + data[0] * b[4];
    res[13] = 0;
    res[12] = 0;
    res[11] = 0;
    res[14] = 0;
    res[15] = data[7] * b[0] + data[4] * b[1] + data[5] * b[2] + data[6] * b[3] + data[3] * b[6] + data[2] * b[5] + data[1] * b[4] + data[0] * b[7];
    return res;
}

// Regressive Product

// MultiVector
[[nodiscard]] MultiVector MultiVector::operator& (const MultiVector& b) const
{
    MultiVector res{};
    res[15] = b[15] * data[15];
    res[14] = b[14] * data[15] + b[15] * data[14];
    res[11] = b[11] * data[15] + b[15] * data[11];
    res[12] = b[12] * data[15] + b[15] * data[12];
    res[13] = b[13] * data[15] + b[15] * data[13];
    res[8] = b[8] * data[15] + b[11] * data[14] - b[14] * data[11] + b[15] * data[8];
    res[9] = b[9] * data[15] + b[12] * data[14] - b[14] * data[12] + b[15] * data[9];
    res[10] = b[10] * data[15] + b[13] * data[14] - b[14] * data[13] + b[15] * data[10];
    res[7] = b[7] * data[15] + b[12] * data[11] - b[11] * data[12] + b[15] * data[7];
    res[6] = b[6] * data[15] - b[13] * data[11] + b[11] * data[13] + b[15] * data[6];
    res[5] = b[5] * data[15] + b[13] * data[12] - b[12] * data[13] + b[15] * data[5];
    res[4] = b[4] * data[15] + b[7] * data[14] - b[9] * data[11] + b[8] * data[12] + b[12] * data[8] - b[11] * data[9] + b[14] * data[7] + b[15] * data[4];
    res[3] = b[3] * data[15] + b[6] * data[14] + b[10] * data[11] - b[8] * data[13] - b[13] * data[8] + b[11] * data[10] + b[14] * data[6] + b[15] * data[3];
    res[2] = b[2] * data[15] + b[5] * data[14] - b[10] * data[12] + b[9] * data[13] + b[13] * data[9] - b[12] * data[10] + b[14] * data[5] + b[15] * data[2];
    res[1] = b[1] * data[15] - b[5] * data[11] - b[6] * data[12] - b[7] * data[13] - b[13] * data[7] - b[12] * data[6] - b[11] * data[5] + b[15] * data[1];
    res[0] = b[0] * data[15] - b[1] * data[14] - b[2] * data[11] - b[3] * data[12] - b[4] * data[13] + b[5] * data[8] + b[6] * data[9] + b[7] * data[10] + b[10] * data[7] + b[9] * data[6] + b[8] * data[5] + b[13] * data[4] + b[12] * data[3] + b[11] * data[2] + b[14] * data[1] + b[15] * data[0];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator& (const ThreeBlade& b) const
{
    MultiVector res{};
    res[14] = b[3] * data[15];
    res[11] = b[0] * data[15];
    res[12] = b[1] * data[15];
    res[13] = b[2] * data[15];
    res[8] = b[0] * data[14] - b[3] * data[11];
    res[9] = b[1] * data[14] - b[3] * data[12];
    res[10] = b[2] * data[14] - b[3] * data[13];
    res[7] = b[1] * data[11] - b[0] * data[12];
    res[6] = -b[2] * data[11] + b[0] * data[13];
    res[5] = b[2] * data[12] - b[1] * data[13];
    res[4] = b[1] * data[8] - b[0] * data[9] + b[3] * data[7];
    res[3] = -b[2] * data[8] + b[0] * data[10] + b[3] * data[6];
    res[2] = b[2] * data[9] - b[1] * data[10] + b[3] * data[5];
    res[1] = -b[2] * data[7] - b[1] * data[6] - b[0] * data[5];
    res[0] = b[2] * data[4] + b[1] * data[3] + b[0] * data[2] + b[3] * data[1];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator& (const TwoBlade& b) const
{
    MultiVector res{};
    res[8] = b[8] * data[15];
    res[9] = b[9] * data[15];
    res[10] = b[10] * data[15];
    res[7] = b[7] * data[15];
    res[6] = b[6] * data[15];
    res[5] = b[5] * data[15];
    res[4] = b[7] * data[14] - b[9] * data[11] + b[8] * data[12];
    res[3] = b[6] * data[14] + b[10] * data[11] - b[8] * data[13];
    res[2] = b[5] * data[14] - b[10] * data[12] + b[9] * data[13];
    res[1] = b[5] * data[11] - b[6] * data[12] - b[7] * data[13];
    res[0] = b[5] * data[8] + b[6] * data[9] + b[7] * data[10] + b[10] * data[7] + b[9] * data[6] + b[8] * data[5];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator& (const OneBlade& b) const
{
    MultiVector res{};
    res[15] = 0;
    res[14] = 0;
    res[11] = 0;
    res[12] = 0;
    res[13] = 0;
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[7] = 0;
    res[6] = 0;
    res[5] = 0;
    res[4] = b[3] * data[15];
    res[3] = b[2] * data[15];
    res[2] = b[1] * data[15];
    res[1] = b[0] * data[15];
    res[0] = - b[0] * data[14] - b[1] * data[11] - b[2] * data[12] - b[3] * data[13];
    return res;
}
[[nodiscard]] MultiVector MultiVector::operator& (const Motor& b) const
{
    MultiVector res{};
    res[15] = b[7] * data[15];
    res[14] = b[7] * data[14];
    res[11] = b[7] * data[11];
    res[12] = b[7] * data[12];
    res[13] = b[7] * data[13];
    res[8] = b[4] * data[15] + b[7] * data[8];
    res[9] = b[5] * data[15] + b[7] * data[9];
    res[10] = b[6] * data[15] + b[7] * data[10];
    res[7] = b[3] * data[15] + b[7] * data[7];
    res[6] = b[2] * data[15] + b[7] * data[6];
    res[5] = b[1] * data[15] + b[7] * data[5];
    res[4] = b[3] * data[14] - b[5] * data[11] + b[4] * data[12] + b[7] * data[4];
    res[3] = b[2] * data[14] + b[6] * data[11] - b[4] * data[13] + b[7] * data[3];
    res[2] = b[1] * data[14] - b[6] * data[12] + b[5] * data[13] + b[7] * data[2];
    res[1] = - b[1] * data[11] - b[2] * data[12] - b[3] * data[13] + b[7] * data[1];
    res[0] = b[0] * data[15] + b[1] * data[8] + b[2] * data[9] + b[3] * data[10] + b[6] * data[7] + b[5] * data[6] + b[4] * data[5] + b[7] * data[0];
    return res;
}
// ThreeBlade
[[nodiscard]] MultiVector ThreeBlade::operator& (const MultiVector& b) const
{
    MultiVector res{};
    res[15] = 0;
    res[14] = data[3] * b[15];
    res[11] = data[0] * b[15];
    res[12] = data[1] * b[15];
    res[13] = data[2] * b[15];
    res[8] = data[0] * b[14] - data[3] * b[11];
    res[9] = data[1] * b[14] - data[3] * b[12];
    res[10] = data[2] * b[14] - data[3] * b[13];
    res[7] = data[1] * b[11] - data[0] * b[12];
    res[6] = -data[2] * b[11] + data[0] * b[13];
    res[5] = data[2] * b[12] - data[1] * b[13];
    res[4] = data[1] * b[8] - data[0] * b[9] + data[3] * b[7];
    res[3] = -data[2] * b[8] + data[0] * b[10] + data[3] * b[6];
    res[2] = data[2] * b[9] - data[1] * b[10] + data[3] * b[5];
    res[1] = -data[2] * b[7] - data[1] * b[6] - data[0] * b[5];
    res[0] = data[2] * b[4] + data[1] * b[3] + data[0] * b[2] + data[3] * b[1];
    return res;
}
[[nodiscard]] TwoBlade ThreeBlade::operator& (const ThreeBlade& b) const
{
    TwoBlade res{};
    res[0] = b[2] * data[1] - b[1] * data[2];
    res[1] = -b[2] * data[0] + b[0] * data[2];
    res[2] = b[1] * data[0] - b[0] * data[1];
    res[3] = b[0] * data[3] - b[3] * data[0];
    res[4] = b[1] * data[3] - b[3] * data[1];
    res[5] = b[2] * data[3] - b[3] * data[2];
    return res;
}
[[nodiscard]] OneBlade ThreeBlade::operator& (const TwoBlade& b) const
{
    OneBlade res{};
    res[3] = data[1] * b[3] - data[0] * b[4] + data[3] * b[2];
    res[2] = -data[2] * b[3] + data[0] * b[5] + data[3] * b[1];
    res[1] = data[2] * b[4] - data[1] * b[5] + data[3] * b[0];
    res[0] = -data[2] * b[2] - data[1] * b[1] - data[0] * b[0];
    return res;
}
[[nodiscard]] float ThreeBlade::operator& (const OneBlade& b) const
{
    return data[2] * b[3] + data[1] * b[2] + data[0] * b[1] + data[3] * b[0];
}
[[nodiscard]] MultiVector ThreeBlade::operator& (const Motor& b) const
{
    MultiVector res{};
    res[15] = 0;
    res[14] = data[3] * b[7];
    res[11] = data[0] * b[7];
    res[12] = data[1] * b[7];
    res[13] = data[2] * b[7];
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[7] = 0;
    res[6] = 0;
    res[5] =0;
    res[4] = data[1] * b[4] - data[0] * b[5] + data[3] * b[3];
    res[3] = -data[2] * b[4] + data[0] * b[6] + data[3] * b[2];
    res[2] = data[2] * b[5] - data[1] * b[6] + data[3] * b[1];
    res[1] = -data[2] * b[3] - data[1] * b[2] - data[0] * b[1];
    res[0] = 0;
    return res;
}
// TwoBlade
[[nodiscard]] MultiVector TwoBlade::operator& (const MultiVector& b) const
{
    MultiVector res{};
    res[15] = 0;
    res[14] = 0;
    res[11] = 0;
    res[12] = 0;
    res[13] = 0;
    res[8] = b[15] * data[3];
    res[9] = b[15] * data[4];
    res[10] = b[15] * data[5];
    res[7] = b[15] * data[2];
    res[6] = b[15] * data[1];
    res[5] = b[15] * data[0];
    res[4] = b[12] * data[3] - b[11] * data[4] + b[14] * data[2];
    res[3] = b[13] * data[3] + b[11] * data[5] + b[14] * data[1];
    res[2] = b[13] * data[4] - b[12] * data[5] + b[14] * data[0];
    res[1] = - b[13] * data[2] - b[12] * data[1] - b[11] * data[0];
    res[0] = b[5] * data[3] + b[6] * data[4] + b[7] * data[5] + b[10] * data[2] + b[9] * data[1] + b[8] * data[0];
    return res;
}
[[nodiscard]] OneBlade TwoBlade::operator& (const ThreeBlade& b) const
{
    OneBlade res{};
    res[3] = b[1] * data[3] - b[0] * data[4] + b[3] * data[2];
    res[2] = b[2] * data[3] + b[0] * data[5] + b[3] * data[1];
    res[1] = b[2] * data[4] - b[1] * data[5] + b[3] * data[0];
    res[0] = -b[2] * data[2] - b[1] * data[1] - b[0] * data[0];
    return res;
}
[[nodiscard]] float TwoBlade::operator& (const TwoBlade& b) const
{
    return b[0] * data[3] + b[1] * data[4] + b[2] * data[5] + b[5] * data[2] + b[4] * data[1] + b[3] * data[0];
}
[[nodiscard]] GANull TwoBlade::operator& (const OneBlade& b) const
{
    return GANull{};
}
[[nodiscard]] MultiVector TwoBlade::operator& (const Motor& b) const
{
    MultiVector res{};
    res[15] = 0;
    res[14] = 0;
    res[11] = 0;
    res[12] = 0;
    res[13] = 0;
    res[8] = b[7] * data[3];
    res[9] = b[7] * data[4];
    res[10] = b[7] * data[5];
    res[7] = b[7] * data[2];
    res[6] = b[7] * data[1];
    res[5] = b[7] * data[0];
    res[4] = 0;
    res[3] = 0;
    res[2] = 0;
    res[1] = 0;
    res[0] = b[1] * data[3] + b[2] * data[4] + b[3] * data[5] + b[6] * data[2] + b[5] * data[1] + b[4] * data[0];
    return res;
}
// Oneblade
[[nodiscard]] MultiVector OneBlade::operator& (const MultiVector& b) const
{
    MultiVector res{};
    res[15] = 0;
    res[14] = 0;
    res[11] = 0;
    res[12] = 0;
    res[13] = 0;
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[7] = 0;
    res[6] = 0;
    res[5] = 0;
    res[4] = b[15] * data[3];
    res[3] = b[15] * data[2];
    res[2] = b[15] * data[1];
    res[1] = b[15] * data[0];
    res[0] = b[13] * data[3] + b[12] * data[2] + b[11] * data[1] + b[14] * data[0];
    return res;
}
[[nodiscard]] float OneBlade::operator& (const ThreeBlade& b) const
{
    return b[2] * data[3] + b[1] * data[2] + b[0] * data[1] + b[3] * data[0];
}
[[nodiscard]] GANull OneBlade::operator& (const TwoBlade& b) const
{
    return GANull{};
}
[[nodiscard]] GANull OneBlade::operator& (const OneBlade& b) const
{
    return GANull{};
}
[[nodiscard]] OneBlade OneBlade::operator& (const Motor& b) const
{
    OneBlade res{};
    res[3] = b[7] * data[3];
    res[2] = b[7] * data[2];
    res[1] = b[7] * data[1];
    res[0] = b[7] * data[0];
    return res;
}
// Motor
[[nodiscard]] MultiVector Motor::operator& (const MultiVector& b) const
{
    MultiVector res{};
    res[15] = b[15] * data[7];
    res[14] = b[14] * data[7];
    res[11] = b[11] * data[7];
    res[12] = b[12] * data[7];
    res[13] = b[13] * data[7];
    res[8] = b[8] * data[7] + b[15] * data[4];
    res[9] = b[9] * data[7] + b[15] * data[5];
    res[10] = b[10] * data[7] + b[15] * data[6];
    res[7] = b[7] * data[7] + b[15] * data[3];
    res[6] = b[6] * data[7] + b[15] * data[2];
    res[5] = b[5] * data[7] + b[15] * data[1];
    res[4] = b[4] * data[7] + b[12] * data[4] - b[11] * data[5] + b[14] * data[3];
    res[3] = b[3] * data[7] - b[13] * data[4] + b[11] * data[6] + b[14] * data[2];
    res[2] = b[2] * data[7] + b[13] * data[5] - b[12] * data[6] + b[14] * data[1];
    res[1] = b[1] * data[7] - b[13] * data[3] - b[12] * data[2] - b[11] * data[1];
    res[0] = b[0] * data[7] + b[5] * data[4] + b[6] * data[5] + b[7] * data[6] + b[10] * data[3] + b[9] * data[2] + b[8] * data[1] + b[15] * data[0];
    return res;
}
[[nodiscard]] MultiVector Motor::operator& (const ThreeBlade& b) const
{
    MultiVector res{};
    res[15] = 0;
    res[14] = b[3] * data[7];
    res[11] = b[0] * data[7];
    res[12] = b[1] * data[7];
    res[13] = b[2] * data[7];
    res[8] = 0;
    res[9] = 0;
    res[10] = 0;
    res[7] = 0;
    res[6] = 0;
    res[5] = 0;
    res[4] = b[12] * data[4] - b[0] * data[5] + b[3] * data[3];
    res[3] = - b[2] * data[4] + b[0] * data[6] + b[3] * data[2];
    res[2] = b[2] * data[5] - b[12] * data[6] + b[3] * data[1];
    res[1] = - b[2] * data[3] - b[12] * data[2] - b[0] * data[1];
    res[0] = b[15] * data[0];
    return res;
}
[[nodiscard]] Motor Motor::operator& (const TwoBlade& b) const
{
    Motor res{};
    res[15] = 0;
    res[8] = b[3] * data[7];
    res[9] = b[4] * data[7];
    res[10] = b[5] * data[7];
    res[7] = b[2] * data[7];
    res[6] = b[1] * data[7];
    res[5] = b[0] * data[7];
    res[0] = b[0] * data[4] + b[1] * data[5] + b[2] * data[6] + b[5] * data[3] + b[4] * data[2] + b[3] * data[1];
    return res;
}
[[nodiscard]] OneBlade Motor::operator& (const OneBlade& b) const
{
    OneBlade res{};
    res[3] = b[3] * data[7];
    res[2] = b[2] * data[7];
    res[1] = b[1] * data[7];
    res[0] = b[0] * data[7];
    return res;
}
[[nodiscard]] Motor Motor::operator& (const Motor& b) const
{
    Motor res{};
    res[7] = b[15] * data[7];
    res[4] = b[4] * data[7] + b[15] * data[4];
    res[5] = b[5] * data[7] + b[15] * data[5];
    res[6] = b[6] * data[7] + b[15] * data[6];
    res[3] = b[3] * data[7] + b[15] * data[3];
    res[2] = b[2] * data[7] + b[15] * data[2];
    res[1] = b[1] * data[7] + b[15] * data[1];
    res[0] = b[0] * data[7] + b[1] * data[4] + b[2] * data[5] + b[3] * data[6] + b[6] * data[3] + b[5] * data[2] + b[4] * data[1] + b[15] * data[0];
    return res;
}

// Dual operator
[[nodiscard]] MultiVector MultiVector::operator! () const
{
    return MultiVector(
        data[15],
        data[14],
        data[11],
        data[12],
        data[13],
        data[8],
        data[9],
        data[10],
        data[5],
        data[6],
        data[7],
        data[2],
        data[3],
        data[4],
        data[1],
        data[0]
    );
}
[[nodiscard]] OneBlade ThreeBlade::operator! () const
{
    return OneBlade(data[3], data[0], data[1], data[2]);
}
[[nodiscard]] TwoBlade TwoBlade::operator! () const
{
    return TwoBlade(
        data[3],
        data[4],
        data[5],
        data[0],
        data[1],
        data[2]
    );
}
[[nodiscard]] ThreeBlade OneBlade::operator! () const
{
    return ThreeBlade(data[1], data[2], data[3], data[0]);
}
[[nodiscard]] Motor Motor::operator! () const
{
    return Motor(
        data[7],
        data[4],
        data[5],
        data[6],
        data[1],
        data[2],
        data[3],
        data[0]
    );
}
