package es.uc3m.g1.musey.model.api

import es.uc3m.g1.musey.model.api.lastfm.SearchResult
import es.uc3m.g1.musey.model.api.lastfm.Similar
import es.uc3m.g1.musey.model.api.lastfm.Track

import retrofit2.Call
import retrofit2.http.GET
import retrofit2.http.Query

interface LastFM {
    @GET("?method=track.getSimilar&format=json")
    fun getSimilar(
        @Query("api_key") key: String,
        @Query("artist") artist: String,
        @Query("track") title: String,
        @Query("limit") limit: Int,
    ): Call<Similar>

    @GET("?method=track.getInfo&format=json")
    fun getInfo(
        @Query("api_key") key: String,
        @Query("artist") artist: String,
        @Query("track") title: String,
    ): Call<Track.Wrapper>

    @GET("?method=track.search&format=json")
    fun search(
        @Query("api_key") key: String,
        @Query("track") title: String,
        @Query("limit") limit: Int,
    ): Call<SearchResult>
}