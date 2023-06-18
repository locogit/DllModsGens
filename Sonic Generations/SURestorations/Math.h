#pragma once
// Skyth
#define DEGREES_TO_RADIANS(x) (float)(x / 180.0f * PI)
#define RADIANS_TO_DEGREES(x) (float)(x / PI * 180.0f)

namespace Ease {
    static float OutQuad(float x) {
        return 1 - (1 - x) * (1 - x);
    }

    static float OutExpo(float x) {
        return x == 1 ? 1 : 1 - std::pow(2, -10 * x);
    }

    static float LinearInterpolate(float a, float b, float t) {
        return (1.0f - t) * a + t * b;
    }
}

namespace Math {
    static float angleBetween(const Hedgehog::Math::CVector& a, const Hedgehog::Math::CVector& b) {
        float cosTheta = a.dot(b) / (a.norm() * b.norm());

        return acos(cosTheta);
    }
}

namespace Eigen
{
    template<typename Scalar>
    static Matrix<Scalar, 4, 4> CreatePerspectiveMatrix(const Scalar fieldOfView, const Scalar aspectRatio, const Scalar zNear, const Scalar zFar) // Skyth
    {
        const Scalar yScale = (Scalar)1 / std::tan(fieldOfView / (Scalar)2);
        const Scalar xScale = yScale / aspectRatio;

        Matrix<Scalar, 4, 4> matrix;

        matrix <<
            xScale, 0, 0, 0,
            0, yScale, 0, 0,
            0, 0, -(zFar + zNear) / (zFar - zNear), -2 * zNear * zFar / (zFar - zNear),
            0, 0, -1, 0;

        return matrix;
    }

    static Vector3f ClampMagnitude(Vector3f v, float m) { // https://github.com/Unity-Technologies/UnityCsReference/blob/master/Runtime/Export/Math/Vector3.cs
        float sqrmag = v.squaredNorm();
        if (sqrmag > m * m)
        {
            float mag = (float)std::sqrt(sqrmag);
            //these intermediate variables force the intermediate result to be
            //of float precision. without this, the intermediate result can be of higher
            //precision, which changes behavior.
            float normalized_x = v.x() / mag;
            float normalized_y = v.y() / mag;
            float normalized_z = v.z() / mag;
            return Eigen::Vector3f(normalized_x * m,
                normalized_y * m,
                normalized_z * m);
        }
        return v;
    }

    static Vector3f Lerp(Vector3f a, Vector3f b, float t)  { // https://github.com/Unity-Technologies/UnityCsReference/blob/master/Runtime/Export/Math/Vector3.cs
        t = std::clamp(t, 0.0f, 1.0f);
        return Vector3f(
            a.x() + (b.x() - a.x()) * t,
            a.y() + (b.y() - a.y()) * t,
            a.z() + (b.z() - a.z()) * t
        );
    }

    static Vector3f SmoothDamp(Vector3f current, Vector3f target, Vector3f& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
    {
        float output_x = 0.0f;
        float output_y = 0.0f;
        float output_z = 0.0f;

        // Based on Game Programming Gems 4 Chapter 1.10
        smoothTime = max(smoothTime, 0.0001F);
        float omega = 2.0F / smoothTime;

        float x = omega * deltaTime;
        float exp = 1.0F / (1.0F + x + 0.48F * x * x + 0.235F * x * x * x);

        float change_x = current.x() - target.x();
        float change_y = current.y() - target.y();
        float change_z = current.z() - target.z();
        Vector3f originalTo = target;

        // Clamp maximum speed
        float maxChange = maxSpeed * smoothTime;

        float maxChangeSq = maxChange * maxChange;
        float sqrmag = change_x * change_x + change_y * change_y + change_z * change_z;
        if (sqrmag > maxChangeSq)
        {
            float mag = (float)std::sqrt(sqrmag);
            change_x = change_x / mag * maxChange;
            change_y = change_y / mag * maxChange;
            change_z = change_z / mag * maxChange;
        }

        target.x() = current.x() - change_x;
        target.y() = current.y() - change_y;
        target.z() = current.z() - change_z;

        float temp_x = (currentVelocity.x() + omega * change_x) * deltaTime;
        float temp_y = (currentVelocity.y() + omega * change_y) * deltaTime;
        float temp_z = (currentVelocity.z() + omega * change_z) * deltaTime;

        currentVelocity.x() = (currentVelocity.x() - omega * temp_x) * exp;
        currentVelocity.y() = (currentVelocity.y() - omega * temp_y) * exp;
        currentVelocity.z() = (currentVelocity.z() - omega * temp_z) * exp;

        output_x = target.x() + (change_x + temp_x) * exp;
        output_y = target.y() + (change_y + temp_y) * exp;
        output_z = target.z() + (change_z + temp_z) * exp;

        // Prevent overshooting
        float origMinusCurrent_x = originalTo.x() - current.x();
        float origMinusCurrent_y = originalTo.y() - current.y();
        float origMinusCurrent_z = originalTo.z() - current.z();
        float outMinusOrig_x = output_x - originalTo.x();
        float outMinusOrig_y = output_y - originalTo.y();
        float outMinusOrig_z = output_z - originalTo.z();

        if (origMinusCurrent_x * outMinusOrig_x + origMinusCurrent_y * outMinusOrig_y + origMinusCurrent_z * outMinusOrig_z > 0)
        {
            output_x = originalTo.x();
            output_y = originalTo.y();
            output_z = originalTo.z();

            currentVelocity.x() = (output_x - originalTo.x()) / deltaTime;
            currentVelocity.y() = (output_y - originalTo.y()) / deltaTime;
            currentVelocity.z() = (output_z - originalTo.z()) / deltaTime;
        }

        return Vector3f(output_x, output_y, output_z);
    }
}