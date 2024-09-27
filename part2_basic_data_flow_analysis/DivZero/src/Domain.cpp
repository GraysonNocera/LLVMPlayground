#include "Domain.h"

//===----------------------------------------------------------------------===//
// Abstract Domain Implementation
//===----------------------------------------------------------------------===//

/* Add your code here */

dataflow::Domain *dataflow::Domain::add(Domain *E1, Domain *E2)
{
    dataflow::Domain::Element e1 = E1->Value;
    dataflow::Domain::Element e2 = E2->Value;
    dataflow::Domain * e3 = new dataflow::Domain();

    if (e1 == dataflow::Domain::Uninit || e2 == dataflow::Domain::Uninit || e1 == dataflow::Domain::MaybeZero || e2 == dataflow::Domain::MaybeZero)
    {
        e3->Value = dataflow::Domain::MaybeZero;
    }
    else if (e1 == dataflow::Domain::Zero && e2 == dataflow::Domain::Zero)
    {
        e3->Value = dataflow::Domain::Zero;
    }
    else if (e1 == dataflow::Domain::NonZero && e2 == dataflow::Domain::NonZero)
    {
        e3->Value = dataflow::Domain::MaybeZero;
    }
    else // at this point, we know one is nonzero and one is zero
    {
        e3->Value = dataflow::Domain::NonZero;
    }

    outs() << "e1, e2 = e3 -> " << e1 << ", " << e2 << " = " << e3->Value << "\n";

    return e3;
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
    dataflow::Domain::Element e2 = E2->Value;
    dataflow::Domain *e3 = new dataflow::Domain();

    // if both 0 => 0
    // if neither 0 => nonZero
    // if either 0 => 0

    if (e1 == dataflow::Domain::Uninit || e2 == dataflow::Domain::Uninit || e1 == dataflow::Domain::MaybeZero || e2 == dataflow::Domain::MaybeZero)
    {
        e3->Value = dataflow::Domain::MaybeZero;
    }
    else if (e1 == dataflow::Domain::Zero && e2 == dataflow::Domain::Zero)
    {
        e3->Value = dataflow::Domain::Zero;
    }
    else if (e1 == dataflow::Domain::NonZero && e2 == dataflow::Domain::NonZero)
    {
        e3->Value = dataflow::Domain::NonZero;
    }
    else // at this point, we know one is nonzero and one is zero
    {
        e3->Value = dataflow::Domain::Zero;
    }

    return e3;
}

dataflow::Domain *dataflow::Domain::div(Domain *E1, Domain *E2)
{
    dataflow::Domain::Element e1 = E1->Value;
    dataflow::Domain::Element e2 = E2->Value;
    dataflow::Domain *e3 = new dataflow::Domain();

    // if both are 0, undefined
    // if either are 0, undefined
    // if neither are 0, nonZero

    if (e1 == dataflow::Domain::Uninit || e2 == dataflow::Domain::Uninit || e1 == dataflow::Domain::MaybeZero || e2 == dataflow::Domain::MaybeZero)
    {
        e3->Value = dataflow::Domain::MaybeZero;
    }
    else if (e2 == dataflow::Domain::Zero)
    {
        // undefined behavior, division by 0 error
        e3->Value = dataflow::Domain::MaybeZero;
    }
    else if (e1 == dataflow::Domain::Zero)
    {
        // at this point, we know e2 is not 0, e1 is 0, thus, 0 / x = 0
        e3->Value = dataflow::Domain::Zero;
    }

    return e3;
}

dataflow::Domain *dataflow::Domain::join(Domain *E1, Domain *E2)
{
    dataflow::Domain::Element e1 = E1->Value;
    dataflow::Domain::Element e2 = E2->Value;
    dataflow::Domain *e3 = new dataflow::Domain();

    // if they are the same, it's just that
    // if one is 0 and one is nonzero, maybezero

    if (e1 == dataflow::Domain::Uninit || e2 == dataflow::Domain::Uninit || e1 == dataflow::Domain::MaybeZero || e2 == dataflow::Domain::MaybeZero)
    {
        e3->Value = dataflow::Domain::MaybeZero;
    } else if (e1 == e2) {
        e3->Value = e1;
    } else {
        e3->Value = dataflow::Domain::MaybeZero;
    }

    return e3;
}

dataflow::Domain *dataflow::Domain::abstract(int i)
{
    dataflow::Domain::Element e1;
    dataflow::Domain *d = new dataflow::Domain();
    if (i == 0) {
        e1 = dataflow::Domain::Zero;
    } else {
        e1 = dataflow::Domain::NonZero;
    }
    d->Value = e1;
    return d;
}

dataflow::Domain::Domain(Element V)
{
    Value = V;
}

dataflow::Domain::Domain()
{
}
