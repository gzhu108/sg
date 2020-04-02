#pragma once
#ifndef sg_microreactor_ReactorFactoryContainer
#define sg_microreactor_ReactorFactoryContainer

#include <map>
#include "Reactor.h"


namespace sg { namespace microreactor
{
    template <typename Key = int32_t, typename InputStream = std::istream>
    class ReactorFactoryContainer
    {
    public:
        ReactorFactoryContainer() {}
        virtual ~ReactorFactoryContainer() {}

        typedef std::function<std::shared_ptr<Reactor>(InputStream&, Connection&)> Factory;

    public:
        virtual void RegisterReactorFactory(const Key& key, Factory factory)
        {
            if (factory == nullptr)
            {
                return;
            }

            auto found = mReactorFactoryTable.find(key);
            if (found == mReactorFactoryTable.end())
            {
                mReactorFactoryTable[key] = factory;
            }
            else
            {
                LOG("Multiple factory found");

                // Overwrite the factory
                found->second = factory;
            }
        }

        virtual Factory GetReactorFactory(const Key& key)
        {
            auto found = mReactorFactoryTable.find(key);
            if (found == mReactorFactoryTable.end())
            {
                return nullptr;
            }

            return found->second;
        }

        virtual typename std::map<Key, Factory>::size_type GetContainerSize()
        {
            return mReactorFactoryTable.size();
        }

    protected:
        std::map<Key, Factory> mReactorFactoryTable;
    };
}}


#endif // sg_microreactor_ReactorFactoryContainer
