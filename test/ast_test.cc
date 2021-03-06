#include <iostream>
#include "gtest/gtest.h"
#include "v8.h"
#include "libplatform/libplatform.h"
#include "v8_test_fixture.h"
#include "src/parsing/parse-info.h"
#include "src/parsing/parser.h"
#include "src/parsing/parsing.h"
#include "src/parsing/scanner-character-streams.h"
#include "src/globals.h"

using namespace v8;
namespace i = v8::internal;

class AstTest : public V8TestFixture {
};

TEST_F(AstTest, ParseInfo) {
  const Isolate::Scope isolate_scope(isolate_);
  const v8::HandleScope handle_scope(isolate_);
  i::Isolate* internal_isolate = asInternal(isolate_);
  i::Factory* factory = internal_isolate->factory();
  i::Handle<i::String> source = factory->NewStringFromStaticChars(
      "const msg = 'testing'");
  i::Handle<i::Script> script = factory->NewScript(source);
  i::ParseInfo parse_info(script);
  EXPECT_TRUE(parse_info.is_toplevel());
  EXPECT_FALSE(parse_info.is_eval());
  EXPECT_FALSE(parse_info.is_strict_mode());
  EXPECT_FALSE(parse_info.is_native());
  EXPECT_FALSE(parse_info.is_module());
  EXPECT_EQ(parse_info.start_position(), 0) << "start line (I think) was not 0";
  EXPECT_EQ(parse_info.end_position(), 0) << "end line (I think) was not 0";
}

TEST_F(AstTest, Parser) {
  const Isolate::Scope isolate_scope(isolate_);
  const v8::HandleScope handle_scope(isolate_);
  i::Isolate* internal_isolate = asInternal(isolate_);
  i::Factory* factory = internal_isolate->factory();
  i::Handle<i::String> source = factory->NewStringFromStaticChars(
      "const msg = 'testing'");
  i::Handle<i::Script> script = factory->NewScript(source);
  i::ParseInfo parse_info(script);
  bool result = i::parsing::ParseProgram(&parse_info, internal_isolate);
  EXPECT_TRUE(result);
  // ParseProgram set parse_info's literal_ which is a AST node
  i::FunctionLiteral* function_literal = parse_info.literal();
  EXPECT_EQ(function_literal->function_type(), i::FunctionLiteral::FunctionType::kAnonymousExpression);
  EXPECT_EQ(function_literal->kind(), i::FunctionKind::kNormalFunction);
}

