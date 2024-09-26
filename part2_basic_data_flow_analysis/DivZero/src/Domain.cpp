#include "Domain.h"

//===----------------------------------------------------------------------===//
// Abstract Domain Implementation
//===----------------------------------------------------------------------===//

/* Add your code here */

dataflow::Domain *dataflow::Domain::add(Domain *E1, Domain *E2)
{
    dataflow::Domain::Element e1 = E1->Value;
    dataflow::Domain::Element e2 = E1->Value;
    dataflow::Domain e3 = dataflow::Domain();

    if (e1 == dataflow::Domain::Uninit || e2 == dataflow::Domain::Uninit || e1 == dataflow::Domain::MaybeZero || e2 == dataflow::Domain::MaybeZero)
    {
        e3.Value = dataflow::Domain::MaybeZero;
    }
    else if (e1 == dataflow::Domain::Zero && e2 == dataflow::Domain::Zero)
    {
        e3.Value = dataflow::Domain::Zero;
    }
    else if (e1 == dataflow::Domain::NonZero && e2 == dataflow::Domain::NonZero)
    {
        e3.Value = dataflow::Domain::MaybeZero;
    }
    else // at this point, we know one is nonzero and one is zero
    {
        e3.Value = dataflow::Domain::NonZero;
    }

    return &e3;
}

dataflow::Domain *dataflow::Domain::sub(Domain *E1, Domain *E2)
{
    // if both 0, result is 0
    // if both nonzero, result is maybeZero
    // if they are different, result is nonZero
    // I think sub same as add

    return dataflow::Domain::add(E1, E2);
}

dataflow::Domain *dataflow::Domain::mul(Domain *E1, Domain *E2)
{
    dataflow::Domain::Element e1 = E1->Value;
    dataflow::Domain::Element e2 = E1->Value;
    dataflow::Domain e3 = dataflow::Domain();

    // if both 0 => 0
    // if neither 0 => nonZero
    // if either 0 => 0

    if (e1 == dataflow::Domain::Uninit || e2 == dataflow::Domain::Uninit || e1 == dataflow::Domain::MaybeZero || e2 == dataflow::Domain::MaybeZero)
    {
        e3.Value = dataflow::Domain::MaybeZero;
    }
    else if (e1 == dataflow::Domain::Zero && e2 == dataflow::Domain::Zero)
    {
        e3.Value = dataflow::Domain::Zero;
    }
    else if (e1 == dataflow::Domain::NonZero && e2 == dataflow::Domain::NonZero)
    {
        e3.Value = dataflow::Domain::NonZero;
    }
    else // at this point, we know one is nonzero and one is zero
    {
        e3.Value = dataflow::Domain::Zero;
    }

    return &e3;
}

dataflow::Domain *dataflow::Domain::div(Domain *E1, Domain *E2)
{
    return nullptr;
}

dataflow::Domain *dataflow::Domain::join(Domain *E1, Domain *E2)
{
    return nullptr;
}
