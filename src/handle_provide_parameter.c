#include "plugin.h"

// PROTOCOL CONSTANT
const uint8_t ETH_ADDRESS[ADDRESS_LENGTH] = {0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                             0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                             0xee, 0xee, 0xee, 0xee, 0xee, 0xee};

static void handle_unsupported_param(ethPluginProvideParameter_t *msg) {
#ifdef DEBUG
    context_t *context = (context_t *) msg->pluginContext;
    PRINTF("Param not supported: %d\n", context->next_param);
#endif

    msg->result = ETH_PLUGIN_RESULT_ERROR;
}

// Copy amount sent parameter to amount_received
static void handle_amount_received(const ethPluginProvideParameter_t *msg, context_t *context) {
    copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
}

// EDIT THIS: Remove this function and write your own handlers!
static void handle_lst_deposit(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->skip_next_param) {
        return;
    }

    switch (context->next_param) {
        case TOKEN_ADDR:
            copy_address(context->token_addr, msg->parameter, sizeof(context->token_addr));
            context->next_param = STAKE_AMOUNT;
            break;

        case STAKE_AMOUNT:
            handle_amount_received(msg, context);
            context->next_param = UNEXPECTED_PARAMETER;
            context->skip_next_param = true;
            break;

        // Keep this
        default:
            handle_unsupported_param(msg);
            break;
    }
}

static void handle_gain_deposit_rseth(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->skip_next_param) {
        return;
    }

    switch (context->next_param) {
        case STAKE_AMOUNT:
            handle_amount_received(msg, context);
            context->next_param = UNEXPECTED_PARAMETER;
            context->skip_next_param = true;
            break;

        // Keep this
        default:
            handle_unsupported_param(msg);
            break;
    }
}

static void handle_kelp_initiate_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->skip_next_param) {
        return;
    }

    switch (context->next_param) {
        case TOKEN_ADDR:
            copy_address(context->token_addr, msg->parameter, sizeof(context->token_addr));
            if (memcmp(context->token_addr, ETH_ADDRESS, sizeof(context->token_addr)) == 0) {
                strlcpy(context->ticker, "ETH", sizeof(context->ticker));
            }
            context->next_param = UNSTAKE_AMOUNT;
            break;
        case UNSTAKE_AMOUNT:
            handle_amount_received(msg, context);
            context->skip_next_param = true;
            break;

        default:
            handle_unsupported_param(msg);
            break;
    }
}

static void handle_gain_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->skip_next_param) {
        return;
    }
    switch (context->next_param) {
        case UNSTAKE_AMOUNT:
            handle_amount_received(msg, context);
            context->next_param = ACCOUNT_ADDR;
            break;

        case ACCOUNT_ADDR:
            copy_address(context->account_addr, msg->parameter, sizeof(context->account_addr));
            context->next_param = ADDITIONAL_ADDR;
            break;

        case ADDITIONAL_ADDR:
            copy_address(context->additional_addr,
                         msg->parameter,
                         sizeof(context->additional_addr));
            context->next_param = UNEXPECTED_PARAMETER;
            context->skip_next_param = true;
            break;

        // Keep this
        default:
            handle_unsupported_param(msg);
            break;
    }
}

static void handle_kelp_claim_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->skip_next_param) {
        return;
    }
    copy_address(context->token_addr, msg->parameter, sizeof(context->token_addr));
    if (memcmp(context->token_addr, ETH_ADDRESS, sizeof(context->token_addr)) == 0) {
        strlcpy(context->ticker, "ETH", sizeof(context->ticker));
    }
    context->skip_next_param = true;
}

void handle_provide_parameter(ethPluginProvideParameter_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: adapt the cases and the names of the functions.
    switch (context->selectorIndex) {
        case GAIN_DEPOSIT_ETH:
        case KELP_ETH_DEPOSIT:
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        case GAIN_DEPOSIT_LST:
        case KELP_LST_DEPOSIT:
            handle_lst_deposit(msg, context);
            break;

        case KELP_INITIATE_WITHDRAW:
            handle_kelp_initiate_withdraw(msg, context);
            break;

        case KELP_CLAIM_WITHDRAW:
            handle_kelp_claim_withdraw(msg, context);
            break;

        case GAIN_DEPOSIT_RSETH:
            handle_gain_deposit_rseth(msg, context);
            break;

        case GAIN_WITHDRAW:
            handle_gain_withdraw(msg, context);
            break;

        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
