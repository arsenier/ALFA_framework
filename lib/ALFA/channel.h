#pragma once

#include "updatable.h"

template <typename T>
class Channel : public Updatable
{
protected:
    T out;
    T *actuator;

public:
    virtual void drive(T in) = 0;
    void update() {}
    T get() { return out; }
};

template <typename T>
class ChannelMaximum : public Channel<T>
{
public:
    ChannelMaximum(T floor)
    {
        this->floor = floor;
        this->inState = floor;
    }

    void drive(T in) override
    {
        inState = max(inState, in);
    }
    void update() override
    {
        this->out = inState;
        inState = floor;
    }

private:
    T floor;
    T inState;
};

template <typename T>
class ChannelMinimum : public Channel<T>
{
public:
    ChannelMinimum(T ceiling)
    {
        this->ceiling = ceiling;
        this->inState = ceiling;
    }

    void drive(T in) override
    {
        inState = min(inState, in);
    }
    void update() override
    {
        this->out = inState;
        inState = ceiling;
    }

private:
    T ceiling;
    T inState;
};

template <typename T>
class ChannelAverage : public Channel<T>
{
public:
    ChannelAverage()
    {
        this->inState = 0;
    }

    void drive(T in) override
    {
        inState += in;
        inCount++;
    }
    void update() override
    {
        this->out = inCount ? inState / inCount : 0;
        inState = 0;
        inCount = 0;
    }

private:
    T inState;
    size_t inCount;
};

template <typename T>
class ChannelSensor : public Channel<T>
{
public:
    ChannelSensor(T *source)
    {
        this->source = source;
    }

    void drive(T in) override {}

    void update() override
    {
        cli();
        this->out = *source;
        sei();
    }

private:
    T *source;
};

/*

CHANNEL(name,
    chtype, datatype, 
)

*/
