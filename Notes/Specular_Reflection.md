# Specular Reflection

## Mirrored Light Reflection

对于光滑的金属材质来说，光线是不会像漫反射那样随机散射的，而是产生反射。关键是：对于一个金属状的镜子，光线具体是怎么反射的呢？向量数学是我们的好朋友:

<div align="center"><img src="Assets/Ray_Reflection.png" width = "65%" ></div>

反射光线的方向用红色表示为 $\boldsymbol v + 2 \boldsymbol b$ 。其中， $\boldsymbol n$ 为单位法向量，但 $\boldsymbol v$ 不一定是。向量 $\boldsymbol b$ 的长度应为 $\boldsymbol v \cdot \boldsymbol n$ 。这里向量 $\boldsymbol v$ 和向量 $\boldsymbol n$ 方向相反，因此需要在这里加一个负号，于是反射光线 $\boldsymbol r$ 有：

$$
\boldsymbol r = \boldsymbol v - 2 (\boldsymbol v \cdot \boldsymbol n) \cdot \boldsymbol n
$$

```cpp
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}
```

```cpp
class metal : public material {
    public:
        metal(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
};
```

## Fuzzy Reflection

我们还可以给反射方向加入一点点随机性，只要在算出反射向量后，在其终点为球心的球内随机选取一个点作为最终的终点:

<div align="center"><img src="Assets/Fuzzy_Reflection.png" width = "65%" ></div>

当然这个球越大，金属看上去就更加模糊(Fuzzy，或者说粗糙)。所以我们这里引入一个变量来表示模糊的程度（Fuzziness），所以当 Fuzz = 0 时不会产生模糊。如果 Fuzz，也就是随机球的半径很大，光线可能会散射到物体内部去。这时候我们可以认为物体吸收了光线。

```cpp
class metal : public material {
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        double fuzz;
};
```

## The End

[返回主页](../README.md)
