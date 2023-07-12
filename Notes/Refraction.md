# Refraction

折射法则是由 Snell 法则定义的：

$$
\eta \cdot \sin{\theta} = \eta^\prime \cdot \sin{\theta^\prime}
$$

其中 $\theta$ 和 $\theta^\prime$ 分别是入射光线和折射光线与法线的夹角， $\theta$ 和 $\theta^\prime$ （读作eta 和 eta prime）是介质的折射率（通常空气为 1.0、玻璃为 1.3-1.7、钻石为 2.4）。

<div align="center"><img src="Assets/Refraction.png" width = "55%" ></div>

以入射和折射方向均“朝外”，且均为单位向量为例。在折射介质部分有一条折射光线 $R^\prime$ 和一个法向量 $n^\prime$ ，其夹角为 $\theta^\prime$ 。我们可以将 $R^\prime$  分解成垂直和水平与法向量的两个分量：

$$
R^\prime = R^{\prime}_\perp + R^{\prime}_\parallel \tag{1}
$$

首先对于 $R^{\prime}_\perp$ ，其模长为：

$$
|R^{\prime}_\perp| = \sin{\theta^\prime} \cdot |R^\prime| = \sin{\theta^\prime} \tag{2}
$$

$R^{\prime}_\perp$ 的方向也就是水平方向，可以用入射向量 $R$ 和法线向量 $n$ 求出。设 $R$ 在垂直于 $n$ 的分量为 $R_\perp$ ，平行于法线向量的 $R_\parallel$ ：

$$
\begin{aligned}
R_\parallel &= \cos{\theta} \cdot n \\
            &= (R \cdot n) \cdot n\\
R_\perp     &= R - R_\parallel \\
            &= R - (R \cdot n) \cdot n  \tag{3} \\
\end{aligned}
$$

$$
|R_\perp| = \sin{\theta} \tag{4}
$$

结合(2)(3)(4)式有：

$$
\begin{aligned}
R^{\prime}_\perp &= |R^{\prime}_\perp| \cdot \frac{-R_\perp}{|R_\perp|}\\
                 &= \frac{\sin{\theta^\prime}}{\sin{\theta}} ((R \cdot n) \cdot n - R) \\
                 &= \frac{\eta}{\eta^\prime} (\cos{\theta} \cdot n - R) \tag{5}
\end{aligned}
$$

再由(1)(5)式有：

$$
R^{\prime}_\parallel = -\sqrt{1 - |R^{\prime}_\perp|^2} \cdot n \tag{6}
$$

根据上述公式, 我们就能写出计算折射光线 $R^\prime$ 的函数（代码中 $R$ 方向与推导相反）：

```cpp
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
```