package es.uc3m.g1.musey.model.firebase.key

import es.uc3m.g1.musey.md5
import java.security.InvalidKeyException
import javax.crypto.BadPaddingException
import javax.crypto.Cipher
import javax.crypto.spec.IvParameterSpec
import javax.crypto.spec.SecretKeySpec

class KeyStoreInstance {

    private fun String.key(): SecretKeySpec {
        val bytes = this.md5()
        return SecretKeySpec(bytes, "AES")
    }

    fun encrypt(password: String, data: ByteArray): Pair<ByteArray, ByteArray> {
        val cipher: Cipher = Cipher.getInstance("AES/CBC/PKCS5Padding")
        cipher.init(Cipher.ENCRYPT_MODE, password.key())
        val ivBytes = cipher.iv
        val encryptedBytes = cipher.doFinal(data)
        return Pair(ivBytes, encryptedBytes)
    }

    fun decrypt(password: String, iv: ByteArray, data: ByteArray): ByteArray {
        val cipher = Cipher.getInstance("AES/CBC/PKCS5Padding")
        val spec = IvParameterSpec(iv)
        cipher.init(Cipher.DECRYPT_MODE, password.key(), spec)
        try {
            return cipher.doFinal(data)
        } catch (e: BadPaddingException) {
            throw InvalidKeyException(e)
        }
    }
}