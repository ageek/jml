/* parameters.cc                                                   -*- C++ -*-
   Jeremy Barnes, 3 November 2009
   Copyright (c) 2009 Jeremy Barnes.  All rights reserved.

   Implementation of the logic in the parameters class.
*/

#include "parameters.h"
#include "parameters_impl.h"
#include "arch/demangle.h"
#include <typeinfo>

namespace ML {


/*****************************************************************************/
/* PARAMETER_VALUE                                                           */
/*****************************************************************************/

Parameter_Value *
Parameter_Value::
compatible_copy(float * first, float * last) const
{
    auto_ptr<Parameter_Value> result(compatible_ref(first, last));
    copy_to(first, last);
    return result.release();
}

Parameter_Value *
Parameter_Value::
compatible_copy(double * first, double * last) const
{
    auto_ptr<Parameter_Value> result(compatible_ref(first, last));
    copy_to(first, last);
    return result.release();
}

Parameters &
Parameter_Value::
parameters()
{
    throw Exception("attempt to obtain parameters from Parameter_Value of "
                    "class " + demangle(typeid(*this).name()));
}

const Parameters &
Parameter_Value::
parameters() const
{
    throw Exception("attempt to obtain parameters from Parameter_Value of "
                    "class " + demangle(typeid(*this).name()));
}

Vector_Parameter &
Parameter_Value::
vector()
{
    throw Exception("attempt to obtain vector from Parameter_Value of "
                    "class " + demangle(typeid(*this).name()));
}

const Vector_Parameter &
Parameter_Value::
vector() const
{
    throw Exception("attempt to obtain vector from Parameter_Value of "
                    "class " + demangle(typeid(*this).name()));
}

Matrix_Parameter &
Parameter_Value::
matrix()
{
    throw Exception("attempt to obtain matrix from Parameter_Value of "
                    "class " + demangle(typeid(*this).name()));
}

const Matrix_Parameter &
Parameter_Value::
matrix() const
{
    throw Exception("attempt to obtain matrix from Parameter_Value of "
                    "class " + demangle(typeid(*this).name()));
}

void
Parameter_Value::
swap(Parameter_Value & other)
{
    name_.swap(other.name_);
}


/*****************************************************************************/
/* PARAMETERS                                                                */
/*****************************************************************************/

float *
Parameters::
copy_to(float * where, float * limit) const
{
    if (where > limit)
        throw Exception("Parameters::copy_to(): passed limit");

    float * current = where;

    for (Params::const_iterator
             it = params.begin(),
             end = params.end();
         it != end;  ++it) {
        current = it->copy_to(current, limit);
    }

    return current;
}

double *
Parameters::
copy_to(double * where, double * limit) const
{
    if (where > limit)
        throw Exception("Parameters::copy_to(): passed limit");

    double * current = where;

    for (Params::const_iterator
             it = params.begin(),
             end = params.end();
         it != end;  ++it) {
        current = it->copy_to(current, limit);
    }

    return current;
}

Parameters *
Parameters::
compatible_ref(float * first, float * last) const
{
    if (last > first)
        throw Exception("Parameters::compatible_ref(): range oob");

    auto_ptr<Parameters_Ref> result(new Parameters_Ref());

    int i = 0;
    for (Params::const_iterator
             it = params.begin(),
             end = params.end();
         it != end;  ++it) {
        size_t np = it->parameter_count();
        if (first + np > last)
            throw Exception("Parameters::compatible_ref(): bad size");

        result->add(i, it->compatible_ref(first, first + np));
    }

    if (last != first)
        throw Exception("Parameters::compatible_ref(): bad size");
    
    return result.release();
}

Parameters *
Parameters::
compatible_ref(double * first, double * last) const
{
    if (last > first)
        throw Exception("Parameters::compatible_ref(): range oob");

    auto_ptr<Parameters_Ref> result(new Parameters_Ref());

    int i = 0;
    for (Params::const_iterator
             it = params.begin(),
             end = params.end();
         it != end;  ++it) {
        size_t np = it->parameter_count();
        if (first + np > last)
            throw Exception("Parameters::compatible_ref(): bad size");

        result->add(i, it->compatible_ref(first, first + np));
    }

    if (last != first)
        throw Exception("Parameters::compatible_ref(): bad size");
    
    return result.release();
}

Parameters *
Parameters::
compatible_copy(float * first, float * last) const
{
    if (last > first)
        throw Exception("Parameters::compatible_copy(): range oob");

    auto_ptr<Parameters_Ref> result(new Parameters_Ref());

    int i = 0;
    for (Params::const_iterator
             it = params.begin(),
             end = params.end();
         it != end;  ++it) {
        size_t np = it->parameter_count();
        if (first + np > last)
            throw Exception("Parameters::compatible_copy(): bad size");

        result->add(i, it->compatible_copy(first, first + np));
    }

    if (last != first)
        throw Exception("Parameters::compatible_copy(): bad size");
    
    return result.release();
}

Parameters *
Parameters::
compatible_copy(double * first, double * last) const
{
    if (last > first)
        throw Exception("Parameters::compatible_copy(): range oob");

    auto_ptr<Parameters_Ref> result(new Parameters_Ref());

    int i = 0;
    for (Params::const_iterator
             it = params.begin(),
             end = params.end();
         it != end;  ++it) {
        size_t np = it->parameter_count();
        if (first + np > last)
            throw Exception("Parameters::compatible_copy(): bad size");

        result->add(i, it->compatible_copy(first, first + np));
    }

    if (last != first)
        throw Exception("Parameters::compatible_copy(): bad size");
    
    return result.release();
}

Parameters &
Parameters::
add(int index, Parameter_Value * param)
{
    bool inserted
        = by_name.insert(make_pair(param->name(), params.size())).second;

    if (!inserted) {
        delete param;
        throw Exception("param with name "
                        + param->name() + " already existed");
    }

    params.push_back(param);

    return *this;
}

void
Parameters::
serialize(DB::Store_Writer & store) const
{
}

void
Parameters::
reconstitute(DB::Store_Reader & store)
{
}

size_t
Parameters::
parameter_count() const
{
    size_t result = 0;

    for (Params::const_iterator it = params.begin(), end = params.end();
         it != end;  ++it) {
        result += it->parameter_count();
    }

    return result;
}

void
Parameters::
fill(float value)
{
}

void
Parameters::
random_fill(float limit, Thread_Context & context)
{
}
    
void
Parameters::
operator -= (const Parameters & other)
{
}

void
Parameters::
operator += (const Parameters & other)
{
}

double
Parameters::
two_norm() const
{
    return 0.0;
}

void
Parameters::
operator *= (double value)
{
}

void
Parameters::
update(const Parameters & other, double learning_rate)
{
}

Parameters &
Parameters::
subparams(int index, const std::string & name)
{
    if (index < 0 || index >= params.size())
        throw Exception("Parameters::subparams(): invalid parameters");
    return params[index].parameters();
}

const Parameters &
Parameters::
subparams(int index, const std::string & name) const
{
    if (index < 0 || index >= params.size())
        throw Exception("Parameters::subparams(): invalid parameters");
    return params[index].parameters();
}

void
Parameters::
add_subparams(int index, Layer & layer)
{
}
    
Parameters::
Parameters(const std::string & name)
    : Parameter_Value(name)
{
}

Parameters::
Parameters(const Parameters & other)
    : Parameter_Value(other.name())
{
}

Parameters &
Parameters::
operator = (const Parameters & other)
{
    return *this;
}

void
Parameters::
swap(Parameters & other)
{
    Parameter_Value::swap(other);
    params.swap(other.params);
}

void
Parameters::
clear()
{
    params.clear();
}


/*****************************************************************************/
/* PARAMETERS_REF                                                            */
/*****************************************************************************/

Parameters_Ref::
Parameters_Ref()
    : Parameters("")
{
}

Parameters_Ref::
Parameters_Ref(const std::string & name)
    : Parameters(name)
{
}


/*****************************************************************************/
/* PARAMETERS_COPY                                                           */
/*****************************************************************************/

template class Parameters_Copy<float>;
template class Parameters_Copy<double>;


} // namespace ML
