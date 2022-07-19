// surface normals visualization

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r) {
        // hard-coded, expanded equation for returning whether 0, 1 or 2
        // intersections exist between the sphere and the ray being cast
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius*radius;
        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) {
                return -1.0;
        } else {
                return (-half_b - std::sqrt(discriminant) ) / a;
        }
}

/*
The ray_color(ray) function linearly blends white and blue depending on the height of the y coordinate
after scaling the ray direction to unit length (so −1.0<y<1.0).
Because we're looking at the y height after normalizing the vector,
you'll notice a horizontal gradient to the color in addition to the vertical gradient.

I then did a standard graphics trick of scaling that to 0.0≤t≤1.0.
When t=1.0 I want blue. When t=0.0

I want white. In between, I want a blend.
This forms a “linear blend”, or “linear interpolation”, or “lerp” for short, between two things.
A lerp is always of the form blendedValue=(1−t)*startValue+t*endValue, with t going from zero to one.
*/
color ray_color(const ray& r) {
        auto t = hit_sphere(point3(0,0,-1), 0.5, r);
        if (t > 0.0) {
                vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
                return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
        }
        vec3 unit_direction = unit_vector(r.direction());                       // ray normalization
        t = 0.5 * (unit_direction.y() + 1.0);                              // scaling t between 0.0~1.0
        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);     // lerp: strtVal = white, endVal = tintedblue
}

int main() {

        // image setup
        const auto aspect_ratio = 16.0 / 9.0;
        const int image_width = 400;
        const int image_height = static_cast<int>(image_width / aspect_ratio);

        // camera config

        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        auto origin = point3(0, 0, 0);
        auto horizontal = vec3(viewport_width, 0, 0);
        auto vertical = vec3(0, viewport_height, 0);
        auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

        // render

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int i = image_height - 1; i>=0; i--) {
                
                std::cerr << "\rScanlines remaining: " << i << ' '<< std::flush;

                for (int j = 0; j < image_width; j++) {
                        
                        auto u = double(j) / (image_width - 1);
                        auto v = double(i) / (image_height - 1);
                        ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);  // ray to current point
                        color pixel_color = ray_color(r);
                        write_color(std::cout, pixel_color);

                }
        }

        std::cerr << "\nDone.\n";

        return 0;
}

