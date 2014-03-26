// Copyright (c) 2013-2014 Flowgrammable.org
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an "AS IS"
// BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing
// permissions and limitations under the License.

#ifndef TINYNBI_H
#define TINYNBI_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Abstract selectors */
#define OFP_SWITCH 1
#define OFP_PORT 2
#define OFP_QUEUE 3
#define OFP_TABLE 4
#define OFP_FLOW 5
#define OFP_AGGREGATE 6
#define OFP_GROUPS 7
#define OFP_GROUP 8
#define OFP_METERS 9
#define OFP_METER 10
#define OFP_MATCH 11
#define OFP_INSTUCTION 12
#define OFP_ACTION 13
#define OFP_ERROR 14
#define OFP_EXTENSION 15

/* Identifiers */
typedef size_t ofp_switch_id; 
typedef size_t ofp_port_id;
typedef size_t ofp_queue_id;
typedef size_t ofp_buffer_id;
typedef size_t ofp_table_id;
typedef size_t ofp_group_id;
typedef size_t ofp_meter_id;

/* Query and control */
int ofp_get(int, ...);
int ofp_set(int, ...);
int ofp_stats(int, ...);
int ofp_acquire(int, ...);
int ofp_release(int, ...);

struct ofp_switch_mod;
struct ofp_port_mod;
struct ofp_table_mod;
struct ofp_flow_mod;
struct ofp_group_mod;
struct ofp_meter_mod;

struct ofp_switch_stats;
struct ofp_port_stats;
struct ofp_flow_stats;
struct ofp_aggregate_stats;
struct ofp_table_stats;
struct ofp_port_stats;
struct ofp_meter_stats;

/* Flow construction */
struct ofp_match;

struct ofp_action;
struct ofp_set_action; 
struct ofp_push_action;
struct ofp_pop_action;
struct ofp_queue_action;
struct ofp_group_action;
struct ofp_output_action;
struct ofp_extension_action;
struct ofp_actions;

struct ofp_instruction;
struct ofp_goto_instruction;
struct ofp_metadata_instruction;
struct ofp_action_instruction;
struct ofp_meter_instruction;
struct ofp_instructions;

int ofp_build_match(ofp_match*, int ...);
int ofp_build_action(ofp_actions*, int, ...);
int ofp_build_instrution(ofp_instructions*, int, ...);


/* Events */
#define OFP_EVENT_SWITCH 0x01
#define OFP_EVENT_PACKET 0x02
#define OFP_EVENT_PORT   0x04
#define OFP_EVENT_FLOW   0x08
/* ... */

struct ofp_event;
typedef size_t ofp_event_mask;
typedef void (*ofp_event_handler)(int, ofp_event*);

int ofp_poll(int, ofp_event*);

int ofp_handle(ofp_switch_id, ofp_event_mask, ofp_event_handler);

#ifdef __cplusplus
}
#endif

#endif
