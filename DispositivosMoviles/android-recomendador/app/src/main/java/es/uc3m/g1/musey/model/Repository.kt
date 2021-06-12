package es.uc3m.g1.musey.model

import android.content.Context
import android.content.SharedPreferences
import android.content.res.Resources
import es.uc3m.g1.musey.R
import es.uc3m.g1.musey.model.api.RetrofitInstance
import es.uc3m.g1.musey.model.api.lastfm.Track
import es.uc3m.g1.musey.model.firebase.FirebaseInstance
import java.security.InvalidKeyException


class Repository(val context: Context) {

    companion object {
        const val DEFAULT_LIMIT = 20
    }

    private val preferences: SharedPreferences = with(context) {
        getSharedPreferences(getString(R.string.preference_file_key), Context.MODE_PRIVATE)
    }
    private val retrofit: RetrofitInstance = RetrofitInstance(context)

    val firebase: FirebaseInstance = FirebaseInstance()

    var key: String
        set(value) = with(preferences.edit()) {
            putString(context.getString(R.string.preference_apikey_key), value)
            apply()
        }
        get() = preferences.getString(context.getString(R.string.preference_apikey_key), null)
            ?: throw Resources.NotFoundException()

    fun getSimilar(track: Track, limit: Int = DEFAULT_LIMIT): List<Track> {
        val call = retrofit.lastfm.getSimilar(key, track.artist.name, track.title, limit)

        val resp = call.execute()

        var response: List<Track> = emptyList()

        if (resp.isSuccessful) {
            response = resp?.body()?.similarTracks?.track ?: response
        }

        return response
    }

    fun getInfo(track: Track): Track? {
        val resp = retrofit.lastfm.getInfo(key, track.artist.name, track.title).execute()
        return resp.body()?.track
    }

    fun getTrack(track: String, limit: Int = DEFAULT_LIMIT): List<Track> {
        val call = retrofit.lastfm.search(key, track, limit)

        val resp = call.execute()

        val response: Array<Track> = emptyArray()

        if (resp.isSuccessful) {
            return (resp?.body()?.tracks ?: response).toList()
        } else {
            throw InvalidKeyException()
        }
    }

    fun backupApiKey(password: String) = firebase.backupApiKey(password, key)

    suspend fun recoverApiKey(password: String): Boolean {
        val recover = firebase.recoverApiKey(password)
        if (recover != null)
            key = recover
        return recover != null
    }
}