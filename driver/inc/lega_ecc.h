/*
 * Copyright (c) 2022 ASR Microelectronics (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _LEGA_ECC_H_
#define _LEGA_ECC_H_
#include "asr_ecc.h"
#ifdef __cplusplus
extern "C"{
#endif

extern const ECPKI_Domain_t *lega_ecpki_getEcDomain(ECPKI_DomainID_t domainId/*!< [in] Index of one of the domain Id (must be one of the supported domains). */);





extern int lega_ecpki_genkeypair(
                        RND_Context_t         *pRndContext,      /*!< [in/out] Pointer to the RND context buffer. */
                        const ECPKI_Domain_t  *pDomain,          /*!< [in]  Pointer to EC domain (curve). */
                        ECPKI_UserPrivKey_t   *pUserPrivKey,     /*!< [out] Pointer to the private key structure. This structure is used as input to the ECPKI cryptographic primitives. */
                        ECPKI_UserPublKey_t   *pUserPublKey,     /*!< [out] Pointer to the public key structure. This structure is used as input to the CPKI cryptographic primitives. */
                        ECPKI_KG_TempData_t   *pTempData,        /*!< [in] Temporary buffers for internal use, defined in ::CRYS_ECPKI_KG_TempData_t. */
                        ECPKI_KG_FipsContext_t   *pFipsCtx       /*!< [in] Pointer to temporary buffer used in case FIPS certification if required. */
);

extern int lega_ecpki_buildprivkey(
                     const ECPKI_Domain_t *pDomain,            /*!< [in] The EC domain (curve). */
                     const uint8_t        *pPrivKeyIn,         /*!< [in] Pointer to private key data. */
                     uint32_t             PrivKeySizeInBytes, /*!< [in] Size of private key data (in bytes). */
                     ECPKI_UserPrivKey_t  *pUserPrivKey        /*!< [out] Pointer to the private key structure. This structure is used as input to the ECPKI cryptographic primitives. */
);

extern int lega_dx_ecpki_buildpubkey(
            const ECPKI_Domain_t   *pDomain,               /*!< [in]  The EC domain (curve). */
            uint8_t                *PublKeyIn_ptr,         /*!< [in]  Pointer to the input public key data, in compressed or uncompressed or hybrid form:
                                           [PC||X||Y] Big-Endian representation, structured according to [IEEE1363], where: 
                                           <ul><li>X and Y are the public key's EC point coordinates. In compressed form, Y is omitted.</li>
                                           <li> The sizes of X and Y are equal to the size of the EC modulus.</li>
                                           <li> PC is a one-byte point control that defines the type of point compression. </li></ul>*/
            uint32_t                PublKeySizeInBytes,    /*!< [in]  The size of public key data (in bytes). */
            USER_EC_PublKeyCheckMode_t       CheckMode,             /*!< [in]  The required level of public key verification (higher verification level means longer verification time):
                                           <ul><li> 0 = preliminary validation. </li>
                                           <li> 1 = partial validation. </li>
                                           <li> 2 = full validation. </li></ul>*/
            ECPKI_UserPublKey_t    *pUserPublKey,          /*!< [out] Pointer to the output public key structure. This structure is used as input to the ECPKI cryptographic primitives. */
            ECPKI_BUILD_TempData_t *pTempBuff              /*!< [in]  Pointer for a temporary buffer required for the build function. */
);

#define  lega_ecpki_build_pubkey_fullcheck(pDomain, PublKeyIn_ptr, PublKeySizeInBytes, pUserPublKey,  pTempBuff) \
   lega_dx_ecpki_buildpubkey((pDomain), (PublKeyIn_ptr), (PublKeySizeInBytes), (USER_ECpublKeyFullCheck), (pUserPublKey),  (pTempBuff))
extern int lega_ecdh_svdp_dh(
                        ECPKI_UserPublKey_t *PartnerPublKey_ptr,       /*!< [in]  Pointer to a partner public key. */
                        ECPKI_UserPrivKey_t *UserPrivKey_ptr,          /*!< [in]  Pointer to a user private key. */
                        uint8_t             *SharedSecretValue_ptr,    /*!< [out] Pointer to an output buffer that will contain the shared secret value. */
                        uint32_t            *SharedSecrValSize_ptr,    /*!< [in/out] Pointer to the size of user-passed buffer (in) and actual size of output of calculated shared secret value(out). */
                        ECDH_TempData_t     *TempBuff_ptr              /*!< [in]  Pointer to a temporary buffer. */);

extern int lega_ecdsa_sign(
                     RND_Context_t             *pRndContext,        /*!< [in/out] Pointer to the RND context buffer. */
                     ECDSA_SignUserContext_t   *pSignUserContext,   /*!< [in/out] Pointer to the user buffer for signing the database. */
                     ECPKI_UserPrivKey_t       *pSignerPrivKey,    /*!< [in]  A pointer to a user private key structure. */
                     ECPKI_HASH_OpMode_t       hashMode,           /*!< [in]  One of the supported SHA-x HASH modes, as defined in ::CRYS_ECPKI_HASH_OpMode_t.\note MD5 is not supported. */
                     uint8_t                   *pMessageDataIn,    /*!< [in] Pointer to the input data to be signed.The size of the scatter/gather list representing the data buffer
                                                                         is limited to 128 entries, and the size of each entry is limited to 64KB (fragments larger than 64KB are broken into fragments <= 64KB). */
                     uint32_t                  messageSizeInBytes, /*!< [in]  Size of message data in bytes. */
                     uint8_t                   *pSignatureOut,      /*!< [in]  Pointer to a buffer for output of signature. */
                     uint32_t                  *pSignatureOutSize   /*!< [in/out] Pointer to the signature size. Used to pass the size of the SignatureOut buffer (in), which must be >= 2
                                                                          * OrderSizeInBytes. When the API returns, it is replaced with the size of the actual signature (out). */
);

extern int lega_ecdsa_verify (
                    ECDSA_VerifyUserContext_t *pVerifyUserContext, /*!< [in] Pointer to the user buffer for signing the database. */
                    ECPKI_UserPublKey_t       *pUserPublKey,       /*!< [in] Pointer to a user public key structure. */
                    ECPKI_HASH_OpMode_t       hashMode,           /*!< [in] One of the supported SHA-x HASH modes, as defined in ::CRYS_ECPKI_HASH_OpMode_t.\note MD5 is not supported. */
                    uint8_t                   *pSignatureIn,       /*!< [in] Pointer to the signature to be verified. */
                    uint32_t                  SignatureSizeBytes, /*!< [in] Size of the signature (in bytes).  */
                    uint8_t                   *pMessageDataIn,     /*!< [in] Pointer to the input data that was signed (same as given to the signing function). The size of the scatter/gather list representing
                                                                         the data buffer is limited to 128 entries, and the size of each entry is limited to 64KB (fragments larger than 64KB are broken into fragments <= 64KB). */
                    uint32_t                  messageSizeInBytes  /*!< [in] Size of the input data (in bytes). */
                    );

#ifdef __cplusplus
}
#endif
#endif // _LEGA_ECC_H_