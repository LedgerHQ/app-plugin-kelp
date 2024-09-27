# Technical Specification

## About

This documentation describes the smart contracts and functions supported by the kelp plugin.

#### KELP

| Network | Contract Name | Smart Contract                               |
| ------- | ------------- | -------------------------------------------- |
| Ethereum  | LRTDepositPool    | `0x036676389e48133B63a802f8635AD39E752D375D` |
| Ethereum  | LRTWithdrawalManager    | `0x62de59c08eb5dae4b7e6f7a8cad3006d6965ec16` |


Functions covered by above contract/s :
|Contract | Function | Selector | Displayed Parameters |
| --- | --- | --- | --- |
|LRTDepositPool | depositETH(uint256 minRSETHAmountExpected, string calldata referralId) payable | `0x72c51c0b`| native_token_amount (eth) |
|LRTDepositPool | depositAsset(address asset, uint256 depositAmount, uint256 minRSETHAmountExpected, string calldata referralId) | `0xc3ae1766`| `asset`, `depositAmount` |
|LRTWithdrawalManager | initiateWithdrawal(address asset, uint256 rsETHUnstaked, string calldata referralId) | `0xc5a67b01`| `asset`, `rsETHUnstaked` |
|LRTWithdrawalManager | completeWithdrawal(address asset, string calldata referralId) | `0xd3a86833`| `asset` |


#### GAIN

| Network | Contract Name | Smart Contract                                   |
| ------- | ------------- | ------------------------------------------------ |
| Ethereum  | rsETHAdapter    | `0xbf28C9FCb12A97441488f9C68FaA49811a98688a` |
| Ethereum  | agETHToken    | `0xe1B4d34E8754600962Cd944B535180Bd758E6c2e`   |


Functions covered by above contract/s :
|Contract | Function | Selector | Displayed Parameters |
| --- | --- | --- | --- |
|rsETHAdapter | depositRsETH(uint256 rsETHAmount, string calldata referralId) | `0xb8aa0db9`| `rsETHAmount` |
|rsETHAdapter | getRSETHWithERC20(address asset, uint256 depositAmount, string calldata referralId) | `0xe8c3516b`| `asset`, `depositAmount` |
|rsETHAdapter | getRSETHWithETH(string calldata referralId) payable | `0x4e3c04bd`| native_token_amount (eth) |
|agETHToken | requestRedeem(uint256 shares, address receiverAddr, address holderAddr) | `0x7d41c86e`| `shares`, `receiverAddr` |
