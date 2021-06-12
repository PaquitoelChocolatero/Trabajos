package es.uc3m.g1.musey.model.firebase

import android.util.Base64
import com.google.firebase.auth.AuthCredential
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.FirebaseUser
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase
import es.uc3m.g1.musey.asyncGet
import es.uc3m.g1.musey.model.firebase.key.KeyStoreInstance
import java.security.InvalidKeyException

class FirebaseInstance {
    private val auth: FirebaseAuth = FirebaseAuth.getInstance()

    private val keystore: KeyStoreInstance = KeyStoreInstance()

    val user: FirebaseUser?
        get() = auth.currentUser

    var database = FirebaseDatabase.getInstance()
    var root = database.reference

    fun signInWithCredential(credential: AuthCredential) =
            auth.signInWithCredential(credential)

    fun signOut() = auth.signOut()

    private fun DatabaseReference.encrypt(password: String, string: String) {
        val pair = keystore.encrypt(password, string.toByteArray())
        child("iv")
                .setValue(Base64.encodeToString(pair.first, Base64.DEFAULT))
        child("data")
                .setValue(Base64.encodeToString(pair.second, Base64.DEFAULT))
    }

    fun backupApiKey(password: String, key: String) {
        val user = this.user ?: throw IllegalStateException("User is null")

        with(root.child(user.uid)) {
            with(child("apikey")) {
                encrypt(password, key)
            }
            with(child("salt")) {
                encrypt(password, user.uid)
            }
        }

    }

    private suspend fun DatabaseReference.decrypt(password: String): ByteArray? {
        val iv: String? = asyncGet("iv")
        val data: String? = asyncGet("data")
        return if (iv == null || data == null) null
        else keystore.decrypt(
                password,
                Base64.decode(iv, Base64.DEFAULT),
                Base64.decode(data, Base64.DEFAULT)
        )
    }

    suspend fun recoverApiKey(password: String): String? {
        val user = this.user ?: throw IllegalStateException("User is null")

        with(root.child(user.uid)) {
            with(child("salt")) {
                val uid = decrypt(password)
                if (uid == null)
                    return null
                else if (String(uid).trim() != user.uid)
                    throw InvalidKeyException()
            }

            with(child("apikey")) {
                val apikey = decrypt(password)
                return if (apikey != null) String(apikey).trim()
                else null
            }
        }
    }
}