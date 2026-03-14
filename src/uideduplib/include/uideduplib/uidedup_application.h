//
// Created by sashko on 13.03.26.
//

#ifndef UIDEDUP_APPLICATION_H
#define UIDEDUP_APPLICATION_H

#include <Poco/Util/Application.h>

using Poco::Util::Application;
using Poco::Util::OptionSet;

class UidedupApplication : public Application
{
  protected:
    void initialize(Application& self) override;
    void uninitialize() override;
    void reinitialize(Application& self) override;
    void defineOptions(OptionSet& options) override;

    void handleHelp(const std::string& name, const std::string& value);

    int main(const ArgVec& argv) override;

  private:
    bool _helpMode{ false };
};

#endif // UIDEDUP_APPLICATION_H
