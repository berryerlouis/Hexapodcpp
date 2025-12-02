#pragma once
#include "CoreInterface.h"
#include "ObservableInterface.h"
#include "ObserverInterface.h"

namespace Core
{
    template<uint8_t MAX_OBSERVERS, class T>
    class Observable : public ObservableInterface<T> {
    public:
        Observable() :
                     mIndexList(0U) {
            for (size_t i = 0; i < MAX_OBSERVERS; i++) {
                mListObserver[i] = nullptr;
            }
        }

        ~Observable() = default;

        virtual Core::Status
        Attach(ObserverInterface<T> *observer) final override {
            Core::Status success = Core::Status::CORE_ERROR;
            if (this->mIndexList != MAX_OBSERVERS) {
                this->mListObserver[this->mIndexList] = observer;
                this->mIndexList += 1U;
                success = Core::Status::CORE_OK;
            }
            return (success);
        }

    protected:
        virtual void
        Notify(const T item) final override {
            for (size_t i = 0; i < this->mIndexList; i++) {
                if (this->mListObserver[i] != nullptr) {
                    this->mListObserver[i]->Notified(item);
                }
            }
        }

    private:
        uint8_t mIndexList;
        ObserverInterface<T> *mListObserver[MAX_OBSERVERS];
    };
}