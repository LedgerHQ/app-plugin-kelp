#include <stdbool.h>
#include "plugin.h"

static bool set_native_token_stake_ui(ethQueryContractUI_t *msg) {
    strlcpy(msg->title, "Stake", msg->titleLength);

    const uint8_t *native_token_amount = msg->pluginSharedRO->txContent->value.value;
    uint8_t native_token_amount_size = msg->pluginSharedRO->txContent->value.length;

    // Converts the uint256 number located in `native_token_amount` to its string representation and
    // copies this to `msg->msg`.
    return amountToString(
        native_token_amount,
        native_token_amount_size,
        WEI_TO_ETHER,
        msg->network_ticker,  // token ticker is same as network ticker for native token staking
        msg->msg,
        msg->msgLength);
}

static bool set_stake_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Stake", msg->titleLength);

    return amountToString(context->amount_received,
                          sizeof(context->amount_received),
                          WEI_TO_ETHER,
                          context->ticker,
                          msg->msg,
                          msg->msgLength);
}

static bool set_claim_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Claim", msg->titleLength);
    strlcpy(msg->msg, context->ticker, msg->msgLength);
    return true;
}

static bool handle_kelp_initiate_withdraw(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Unstake", msg->titleLength);
            ret = amountToString(context->amount_received,
                                 sizeof(context->amount_received),
                                 WEI_TO_ETHER,
                                 "rsETH",
                                 msg->msg,
                                 msg->msgLength);
            break;

        case 1:
            strlcpy(msg->title, "Asset Expected", msg->titleLength);
            strlcpy(msg->msg, context->ticker, msg->msgLength);
            ret = true;
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
    }
    return ret;
}

static bool set_account_addr_ui(ethQueryContractUI_t *msg, context_t *context) {
    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` means it works with any chainID :)
    uint64_t chainid = 0;

    // Get the string format of the address stored in `context->beneficiary`. Store it in
    // `msg->msg`.
    return getEthAddressStringFromBinary(
        context->account_addr,
        (char *) msg->msg + 2,  // +2 because we've already prefixed with '0x'.
        chainid);
}

static bool handle_gain_withdraw(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Withdraw", msg->titleLength);
            ret = amountToString(context->amount_received,
                                 sizeof(context->amount_received),
                                 WEI_TO_ETHER,
                                 "agETH",
                                 msg->msg,
                                 msg->msgLength);
            break;

        case 1:
            strlcpy(msg->title, "Receiver", msg->titleLength);
            ret = set_account_addr_ui(msg, context);
            break;

        case 2:
            strlcpy(msg->title, "Asset Expected", msg->titleLength);
            strlcpy(msg->msg, "rsETH", msg->msgLength);
            ret = true;
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
    }
    return ret;
}

void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    bool ret = false;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    // EDIT THIS: Adapt the cases for the screens you'd like to display.
    switch (context->selectorIndex) {
        case GAIN_DEPOSIT_LST:
        case GAIN_DEPOSIT_RSETH:
        case KELP_LST_DEPOSIT:
            ret = set_stake_ui(msg, context);
            break;

        case KELP_CLAIM_WITHDRAW:
            ret = set_claim_ui(msg, context);
            break;

        case GAIN_DEPOSIT_ETH:
        case KELP_ETH_DEPOSIT:
            ret = set_native_token_stake_ui(msg);
            break;

        case KELP_INITIATE_WITHDRAW:
            ret = handle_kelp_initiate_withdraw(msg, context);
            break;

        case GAIN_WITHDRAW:
            ret = handle_gain_withdraw(msg, context);
            break;

        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
