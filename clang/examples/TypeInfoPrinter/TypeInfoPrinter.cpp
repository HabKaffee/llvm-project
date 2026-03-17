#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>

using namespace clang;

class TypeInfoPrinter : public ASTConsumer {
public:
  TypeInfoPrinter() {
    llvm::outs() << "Плагин инициализирован\n";
  }
  
  ~TypeInfoPrinter() {
    llvm::outs() << "Плагин завершил работу\n";
  }
  
  void HandleTranslationUnit(ASTContext &Context) override {
    llvm::outs() << "Плагин: анализ кода...\n";
    llvm::outs() << "Плагин: найденные типы будут обработаны\n";
    llvm::outs() << "Плагин: анализ завершен\n";
  }
};

class TypeInfoAction : public PluginASTAction {
public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef InFile) override {
    llvm::outs() << "Создание потребителя AST для файла: " << InFile << "\n";
    return std::make_unique<TypeInfoPrinter>();
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string>& args) override {
    return true;
  }

  ActionType getActionType() override {
    return AddBeforeMainAction;
  }
};

static FrontendPluginRegistry::Add<TypeInfoAction> 
X("type-info-printer", "Плагин для вывода информации о типах");
