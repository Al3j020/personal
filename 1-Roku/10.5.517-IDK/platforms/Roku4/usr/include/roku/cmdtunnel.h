/*
 * (c) 2018 Roku, Inc.  All content herein is protected by U.S.
 * copyright and other applicable intellectual property laws and may not be
 * copied without the express permission of Roku, Inc., which reserves all
 * rights.  Reuse of any of this content for any purpose without the
 * permission of Roku, Inc.  is strictly prohibited.
 */

#ifndef Scripting_NDK_CommandTunnel_h_
#define Scripting_NDK_CommandTunnel_h_

#include <memory>
#include <functional>
#include <map>


// A command transport tunnel with synchronous or asynchronous replies.
// Commands have the following features:
// * Commands with registered IDs may be sent between any channel context (NDK/Brightscript) and RokuOS.
// * Commands with private IDs may be sent between any context (NDK/Brightscript) within the same channel.
// * The contents of the command are opaque to the RokuOS and may be changed without firmware knowledge provided both ends
//    of the tunnel agree on the content.
// * Replies can be synchronous or asynchronous. The protocol used by both ends of the tunnel must match.

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _CmdTunnelArg {
    const char *name;
    const char *value;
} CmdTunnelArg;

typedef struct _CmdTunnel_CmdInfo {
    uint32_t source;
    uint32_t id;
    uint32_t sequence;
} CmdTunnel_CmdInfo;


typedef void (*CmdTunnel_ReplyHandler)(uint32_t reply_id, uint32_t sequence, const CmdTunnelArg *args);

typedef void (*CmdTunnel_CmdHandler)(CmdTunnel_CmdInfo info, struct ReplyContext *ctx, const CmdTunnelArg *args);

int RokuCmdTunnel_init();
int RokuCmdTunnel_fini();

// Command ID:
//             0: Command ID 0 is reserved
//  1-0x7fffffff: Commands defined by channel for communication between channel components. Contents are
//                unknown to RokuOS.
//   >0x7fffffff: Commands registered with RokuOS for communication between channel and RokuOS or between
//                components of different channels.

// Send a command
// Arguments are null terminated
// If the command has a synchronous reply and reply != null the reply arguments will be returned.
// Reply arguments must then be freed with RokuCmdTunnel_Free()
// If the command has an asynchronous reply the returned sequence ID will match a reply sent to the reply handler.
// A sequence ID of 0 indicates no reply is expected.
uint32_t RokuCmdTunnel_SendCommand(uint32_t id, const CmdTunnelArg *args, CmdTunnelArg **reply);
void RokuCmdTunnel_Free(CmdTunnelArg *args);

// Install a handler for Asynchronous replies
void RokuCmdTunnel_HandleReply(CmdTunnel_ReplyHandler handler);

// This registers a handler to be called when the designated command is received.
// The handler should call RokuCmdTunnel_Reply before returning for synchronous replies.
void RokuCmdTunnel_HandleCommand(uint32_t id, CmdTunnel_CmdHandler handler);
// Provide a synchronous reply. This should be called within the command handler and
// pass the context given to the handler.
void RokuCmdTunnel_Reply(struct ReplyContext *ctx, const CmdTunnelArg *args);

// Send an asynchronous reply to the command indicated by info.
void RokuCmdTunnel_SendReply(const CmdTunnel_CmdInfo *info, const CmdTunnelArg *args);


#ifdef __cplusplus
}
#endif


#endif
