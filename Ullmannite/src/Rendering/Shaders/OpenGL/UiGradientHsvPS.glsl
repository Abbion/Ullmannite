#version 460 core

out vec4 FragColor;

vec3 HsvToRgb(const vec3 hsvValue)
{
    const float C = hsvValue.b * hsvValue.g;
    const float X = C * (1.0f - abs(mod(hsvValue.r / 60.0, 2.0f) - 1.0f));
    const float H = hsvValue.r / 60.0f;

    vec3 rgbOutput = vec3(0.0f, 0.0f, 0.0f);

    if (H >= 0.0f && H < 1.0f) {
        rgbOutput.r = C;
        rgbOutput.g = X;
    }
    else if (H >= 1.0f && H < 2.0f) {
        rgbOutput.r = X;
        rgbOutput.g = C;
    } 
    else if (H >= 2.0f && H < 3.0f) {
        rgbOutput.g = C;
        rgbOutput.b = X;
    }
    else if (H >= 3.0f && H < 4.0f) {
        rgbOutput.g = X;
        rgbOutput.b = C;
    }
    else if (H >= 4.0f && H < 5.0f) {
        rgbOutput.r = X;
        rgbOutput.b = C;
    }
    else if (H >= 5.0f && H < 6.0f) {
        rgbOutput.r = C;
        rgbOutput.b = X;
    }

    const float m = hsvValue.b - C;

    return rgbOutput + vec3(m, m, m);
}

in VS_OUT{
    vec4 color;
} vs_out;

void main()
{
	FragColor = vec4(HsvToRgb(vs_out.color.rgb), vs_out.color.a);
}