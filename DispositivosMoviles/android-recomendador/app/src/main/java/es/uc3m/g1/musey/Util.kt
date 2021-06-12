package es.uc3m.g1.musey

import android.content.Context
import android.os.Looper
import android.view.View
import android.view.inputmethod.InputMethodManager
import android.widget.Toast
import com.google.firebase.database.DatabaseReference
import java.security.MessageDigest
import kotlin.coroutines.resume
import kotlin.coroutines.resumeWithException
import kotlin.coroutines.suspendCoroutine

fun View.hideKeyboard() {
    val imm = context.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
    imm.hideSoftInputFromWindow(windowToken, 0)
}

suspend fun DatabaseReference.asyncGet(key: String): String? {
    return suspendCoroutine { cont ->
        this.child(key).get()
            .addOnSuccessListener { data ->
                if (data.value != null)
                    cont.resume(data.value.toString())
                else
                    cont.resume(null)
            }
            .addOnFailureListener { exception ->
                cont.resumeWithException(exception)
            }
    }
}

fun String.md5(): ByteArray {
    val md: MessageDigest = MessageDigest.getInstance("MD5")
    return md.digest(toByteArray())
}

fun View.talk(id: Int) {
    if (Looper.myLooper() == null) {
        Looper.prepare()
    }
    Toast.makeText(
        context.applicationContext,
        context.getString(id),
        Toast.LENGTH_SHORT
    ).show()
}