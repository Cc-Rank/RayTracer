# Ray Casting

光线在空间中可以表述为起点 $R(t) = O + tD$ ，这里 $R$ 是 3D 射线上的一个点。 $O$ 是射线的原点， $D$ 是射线的方向。

## Sending Rays Into the Scene

> At the core, the ray tracer sends rays through pixels and computes the color seen in the direction of those rays.

光线追踪的核心就是通过像素发射光线，并计算这些光线方向上得到的颜色。

在 GAMES 101 的笔记中，我写过一篇 [笔记](https://github.com/Cc-Rank/GAMES101/tree/main/Assignment/Assignment5#2-%E7%94%9F%E6%88%90%E6%91%84%E5%83%8F%E6%9C%BA%E5%B0%84%E7%BA%BF-generating-camera-rays)，详细介绍了光线从摄像机出发，经过屏幕空间转换至 NDC 空间，再从 NDC 空间转换至世界空间中的过程。

但这里的实现摒弃了那些复杂的空间变换的概念，并直接从屏幕中的像素坐标转换到了具体的成像平面（ViewPort）。

同样的，摄像机被放在 $(0, 0, 0)$ 的位置，y 轴向上 x 轴向右，为了遵守右手系规范，摄像机看向 z 轴负方向，且成像平面距离摄像机的距离为 $1$ 个单位。

<div align="center"><img src="Assets/Camera_Geometry.png" width = "65%" ></div>

这里射线从图像的左下角开始，沿着坐标轴逐渐遍历整个成像平面：

```cpp
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
```

值得注意的是，这里并没有将射线单位化。这是因为这些射线在之后的 [求交过程](./Intersection.md) 中，只需要求出距离出发点最近的一条射线即可。这对于射线来说，并没有单位化的需求，因此这里节省了一些非必要的计算开销。

## Antialiasing

如果一个像素只发出一条射线的话，最终结果对于光源的采样率是极低的，这会导致最终渲染结果有着很明显的走样现象。因此，为了反走样，也就是抗锯齿，需要增加采样频率。

[rtweekend.h](../RayTracer/src/include/rtweekend.h) 中给出了两种生成随机双精度浮点数的方法：`cstdlib` 中的 `rand()`，和 C++标准库`random` 中的 `std::uniform_real_distribution<double>`。

```cpp
    // Render
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
```

## The End

[返回主页](../README.md)
