package es.uc3m.g1.musey.model.api.lastfm

import com.google.gson.annotations.SerializedName

data class Similar (
    @SerializedName("similartracks") var similarTracks: SimilarTracks
) {
    data class SimilarTracks (
        @SerializedName("track") var track: List<Track>,
    )
}