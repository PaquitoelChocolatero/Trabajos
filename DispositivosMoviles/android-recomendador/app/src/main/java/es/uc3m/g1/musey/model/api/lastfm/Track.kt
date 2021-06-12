package es.uc3m.g1.musey.model.api.lastfm

import com.google.gson.annotations.SerializedName
import java.io.Serializable

data class Track(
        @SerializedName("name") var title: String,
        @SerializedName("artist") var artist: Artist,
        @SerializedName("album") var album: Album? = null,
) : Serializable {
    data class Wrapper(
            val track: Track
    )
}