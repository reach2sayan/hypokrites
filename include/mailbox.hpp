//
// Created by sayan on 7/10/25.
//

#ifndef MAILBOX_HPP
#define MAILBOX_HPP
#pragma once
#include "actor.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <functional>
#include <stack>

using mail_id_t = boost::uuids::uuid;

class Mail {
  ActorBase* actor;
  mail_id_t id;
  std::stack<std::function<void()>> stages;
  std::string content;
};



#endif //MAILBOX_HPP
