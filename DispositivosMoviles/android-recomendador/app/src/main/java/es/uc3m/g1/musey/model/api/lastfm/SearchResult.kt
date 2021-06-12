package es.uc3m.g1.musey.model.api.lastfm

import com.google.gson.Gson
import com.google.gson.JsonDeserializationContext
import com.google.gson.JsonDeserializer
import com.google.gson.JsonElement
import com.google.gson.annotations.JsonAdapter
import java.lang.reflect.Type

@JsonAdapter(SearchResult.Deserializer::class)
class SearchResult(
    var tracks: Array<Track> = emptyArray()
) {
    class Deserializer : JsonDeserializer<SearchResult> {
        override fun deserialize(
            json: JsonElement?,
            typeOfT: Type?,
            context: JsonDeserializationContext?
        ): SearchResult {
            val results = json?.asJsonObject?.get("results")
            val matches = results?.asJsonObject?.get("trackmatches")
            val jsonArray = matches?.asJsonObject?.get("track")

            return SearchResult(
                Gson().fromJson(jsonArray, Array<Track>::class.java)
            )
        }
    }
}