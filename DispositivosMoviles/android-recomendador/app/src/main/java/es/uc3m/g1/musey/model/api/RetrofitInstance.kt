package es.uc3m.g1.musey.model.api

import android.content.Context
import android.net.ConnectivityManager
import android.net.Network
import android.net.NetworkCapabilities
import okhttp3.Cache
import okhttp3.OkHttpClient
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

class RetrofitInstance(context: Context) {
    companion object {
        private const val LASTFM_URL = "https://ws.audioscrobbler.com/2.0/"
    }

    private val cacheSize = (5 * 1024 * 1024).toLong()
    private val cache = Cache(context.cacheDir, cacheSize)

    private var internet = false

    init {
        val connectivityManager = context.getSystemService(ConnectivityManager::class.java)
        val network = connectivityManager.activeNetwork
        val capabilities = connectivityManager.getNetworkCapabilities(network)

        if (capabilities != null) {
            internet = capabilities.hasCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
        }

        connectivityManager.registerDefaultNetworkCallback(
                object : ConnectivityManager.NetworkCallback() {
                    private var internet = false

                    override fun onCapabilitiesChanged(
                            network: Network,
                            networkCapabilities: NetworkCapabilities
                    ) {
                        super.onCapabilitiesChanged(network, networkCapabilities)
                        internet = networkCapabilities
                                .hasCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
                        this@RetrofitInstance.internet = internet
                    }

                    override fun onBlockedStatusChanged(network: Network, blocked: Boolean) {
                        super.onBlockedStatusChanged(network, blocked)
                        if (blocked) {
                            internet = this@RetrofitInstance.internet
                            this@RetrofitInstance.internet = false
                        } else {
                            this@RetrofitInstance.internet = internet
                        }
                    }
                }
        )
    }

    private val retrofit by lazy {
        val okHttpClient = OkHttpClient.Builder()
                .cache(cache)
                .addInterceptor { chain ->
                    var request = chain.request()
                    request = if (internet)
                        request.newBuilder().header("Cache-Control", "public, max-age=" + 5).build()
                    else
                        request.newBuilder().header("Cache-Control", "public, only-if-cached, max-stale=" + 60 * 60 * 24 * 7).build()
                    chain.proceed(request)
                }
                .build()
        Retrofit.Builder()
                .baseUrl(LASTFM_URL)
                .addConverterFactory(GsonConverterFactory.create())
                .client(okHttpClient)
                .build()
    }

    val lastfm: LastFM by lazy {
        retrofit.create(LastFM::class.java)
    }
}