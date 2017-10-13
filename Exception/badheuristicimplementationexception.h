#ifndef BADHEURISTICIMPLEMENTATIONEXCEPTION_H
#define BADHEURISTICIMPLEMENTATIONEXCEPTION_H


class BadHeuristicImplementationException : public QException
{
public:
    void raise() const { throw *this; }
    BadHeuristicImplementationException *clone() const { return new BadHeuristicImplementationException(*this); }
};

#endif // BADHEURISTICIMPLEMENTATIONEXCEPTION_H
