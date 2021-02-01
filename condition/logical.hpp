#ifndef __LOGICAL_HPP__
#define __LOGICAL_HPP__

struct Logical {
    virtual bool operator()(bool, bool) const = 0;
    virtual Logical *copy() const = 0;
};

struct And : Logical {
    bool operator()(bool, bool) const override;
    Logical *copy() const override;
};

struct Or : Logical {
    bool operator()(bool, bool) const override;
    Logical *copy() const override;
};

struct Xor : Logical {
    bool operator()(bool, bool) const override;
    Logical *copy() const override;
};

#endif // __LOGICAL_HPP__
