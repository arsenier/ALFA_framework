#pragma once

class ChannelNoT
{
public:
    virtual void update() = 0;
};

template <typename T>
class Channel : public ChannelNoT
{
protected:
    T out;

public:
    virtual void drive(T in) = 0;
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
        this->out = inState / inCount;
        inState = 0;
        inCount = 0;
    }

private:
    T inState;
    size_t inCount;
};
