//
// Created by borisshapa on 10.04.2021.
//

#ifndef IMAGE_SCALING_KERNEL_H
#define IMAGE_SCALING_KERNEL_H

struct Kernel {
    virtual ~Kernel() = default;

    double GetLowerBound() const;

    double GetUpperBound() const;

    virtual double evaluate(double x) const = 0;

protected:
    Kernel(double lower_bound, double upper_bound);

private:
    double lower_bound_ = 0;
    double upper_bound_ = 0;
};

#endif //IMAGE_SCALING_KERNEL_H
