# Intersection

前文我们提到，光线在空间中可以表述为起点 $\boldsymbol P(t) = \boldsymbol O + t \boldsymbol d$ ，这里 $\boldsymbol P$ 是 3D 射线上的一个点。 $\boldsymbol O$ 是射线的原点， $\boldsymbol d$ 是射线的方向。

## Ray-Sphere Intersection

球体，是隐式曲面中最常用到的形状。对于一个球体来说，其表面上所有点 $\boldsymbol p$ ，其到圆心 $O$ 的距离时固定为半径 $r$ 的，也就得到了球的隐式曲面方程：

$$
(\boldsymbol P - \boldsymbol C)(\boldsymbol P - \boldsymbol C) = r^2
$$

当射线与球体相交是，满足射线方程上的一点 $P$ 一定也同时位于球面上。因此我们需要求得满足这一点的任意 $t$ 值：

$$
(\boldsymbol O + t \boldsymbol d - \boldsymbol C) \cdot (\boldsymbol O + t \boldsymbol d - \boldsymbol C) = r^2
$$

如果展开这个方程并把所有的项都移到左边我们得到：

$$
t^2 \boldsymbol d \cdot \boldsymbol d + 2t \boldsymbol d \cdot (\boldsymbol O - \boldsymbol C) + (\boldsymbol O - \boldsymbol C)(\boldsymbol O - \boldsymbol C) = r^2
$$

方程中的向量和半径 $r$ 都是已知的常量，唯一的未知数就是 $t$ ，并且这个等式是关于 $t$ 的一个一元二次方程，这样很容易我们就可以解出有：

```cpp
bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
     if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) ) / (2.0*a);
    }
}
```

## Simplifying the Ray-Sphere Intersection Code

首先，一个向量与自身点积等于这个向量长度的平方。

其次，注意其实我们的 $b$ 有一个系数 $2$ ，我们设 $b=2h$，有：

$$
\begin{aligned}
& \frac{-b \pm \sqrt{b^2 - 4ac}}{2a} \\
&= \frac{-2h \pm \sqrt{(2h)^2 - 4ac}}{2a} \\
&= \frac{-2h \pm 2 \sqrt{h^2 - ac}}{2a} \\
&= \frac{-h \pm \sqrt{h^2 - ac}}{a} \\
\end{aligned}
$$

通过这一观察，我们可以将射线与球体求交的代码简化为：

```cpp
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}
```

## The End

[返回主页](../README.md)
