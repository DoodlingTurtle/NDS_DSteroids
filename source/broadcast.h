#ifndef BROADCAST_H
#define BROADCAST_H

#include <vector>
#include <map>

class Broadcast {

    public:

        class Listener {
        public:
            virtual void onBroadcast(int channel, int eventcode, void* broadcaster);
        };

        Broadcast(int channel);
        virtual ~Broadcast();

        bool subscribe(Listener* l);
        bool unsubscribe(Listener* l);

        void transmit(int eventcode, void* broadcaster);

    protected:

    private:
        static std::map<const int, Broadcast*> channels;
        std::vector<Listener*> listeners;
        int channel = -1;
};

#endif // BROADCAST_H
